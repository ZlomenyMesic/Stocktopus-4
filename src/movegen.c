//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

# include <stdio.h>

#include "movegen.h"
#include "bitboard.h"
#include "move.h"

ulong white_pawn_pushes[64];
ulong black_pawn_pushes[64];
ulong white_pawn_attacks[64];
ulong black_pawn_attacks[64];
ulong knight_moves[64];

void init_moves() {
    init_pawn_moves();
    init_knight_moves();
}

void init_pawn_moves() {
    for (int i = 0; i < 64; i++) {
        if (i < 8 || i > 55) {
            white_pawn_pushes[i] = 0;
            black_pawn_pushes[i] = 0;
            white_pawn_attacks[i] = 0;
            black_pawn_attacks[i] = 0;
        }
        else {
            ulong w_single_step = (ulong)1 << (i - 8);
            ulong b_single_step = (ulong)1 << (i + 8);

            ulong w_double_step = w_single_step >> 8 & 0x000000FF00000000;
            ulong b_double_step = b_single_step << 8 & 0x00000000FF000000;

            ulong w_west_attacks = (ulong)1 << (i - 7) & 0xFEFEFEFEFEFEFEFE;
            ulong b_west_attacks = (ulong)1 << (i + 9) & 0xFEFEFEFEFEFEFEFE;

            ulong w_east_attacks = (ulong)1 << (i - 9) & 0x7F7F7F7F7F7F7F7F;
            ulong b_east_attacks = (ulong)1 << (i + 7) & 0x7F7F7F7F7F7F7F7F;

            white_pawn_pushes[i] = w_single_step | w_double_step;
            black_pawn_pushes[i] = b_single_step | b_double_step;
            white_pawn_attacks[i] = w_west_attacks | w_east_attacks;
            black_pawn_attacks[i] = b_west_attacks | b_east_attacks;
        }
    }
}

void init_knight_moves() {
    for (int i = 0; i < 64; i++) {
        ulong knight = (ulong)1 << i;
        ulong east = knight >> 1 & 0x7F7F7F7F7F7F7F7F;
        ulong west = knight << 1 & 0xFEFEFEFEFEFEFEFE;
        ulong targets = ((east | west) << 16) | ((east | west) >> 16);

        east = east >> 1 & 0x7F7F7F7F7F7F7F7F;
        west = west << 1 & 0xFEFEFEFEFEFEFEFE;
        knight_moves[i] = targets | ((east | west) << 8) | ((east | west) >> 8);
    }
}

int get_legal_moves(Move** moves, const Board* board, Color color) {
    int move_count = 0;
    move_count = get_knight_moves(board, color, moves, move_count);
    move_count = get_pawn_moves(board, color, moves, move_count);
    return move_count;
}

int get_pawn_moves(const Board* board, Color color, Move** moves, int index) {
    ulong pawns = board->pieces[color][0];
    while (pawns) {
        int start = bit_scan_forward_reset(&pawns);
        ulong targets = get_pawn_targets((ulong)1 << start, board, color);
        while (targets) {
            int end = bit_scan_forward_reset(&targets);
            if (end < 8 || end > 55) {
                moves[index++] = move_new(start, end, PieceType_PAWN, board->mailbox[end]->pieceType, PieceType_KNIGHT);
                moves[index++] = move_new(start, end, PieceType_PAWN, board->mailbox[end]->pieceType, PieceType_BISHOP);
                moves[index++] = move_new(start, end, PieceType_PAWN, board->mailbox[end]->pieceType, PieceType_ROOK);
                moves[index++] = move_new(start, end, PieceType_PAWN, board->mailbox[end]->pieceType, PieceType_QUEEN);
            } else moves[index++] = move_new(start, end, PieceType_PAWN, board->mailbox[end]->pieceType, PieceType_NONE);
        }
    }
    return index;
}

int get_knight_moves(const Board* board, Color color, Move** moves, int index) {
    ulong knights = board->pieces[color][1];
    while (knights) {
        int start = bit_scan_forward_reset(&knights);
        ulong targets = get_knight_targets((ulong)1 << start, board, color);
        while (targets) {
            int end = bit_scan_forward_reset(&targets);
            moves[index++] = move_new(start, end, PieceType_KNIGHT, board->mailbox[end]->pieceType, PieceType_NONE);
        }
    }
    return index;
}

ulong get_pawn_targets(ulong pawn, const Board* board, Color color) {
    int index = bit_scan_forward(pawn);
    ulong push_targets = color == Color_WHITE ? white_pawn_pushes[index] : black_pawn_pushes[index];
    ulong atk_targets = color == Color_WHITE ? white_pawn_attacks[index] : black_pawn_attacks[index];
    return (push_targets & board->empty) | (atk_targets & board->occupied[color == Color_WHITE ? 1 : 0]);
}

ulong get_knight_targets(ulong knight, const Board* board, Color color) {
    return knight_moves[bit_scan_forward(knight)] & (board->empty | board->occupied[color == Color_WHITE ? 1 : 0]);
}

int get_square_index(int file, int rank) {
    return rank * 8 + file;
}

int get_rank_index(int square) {
    return square >> 3;
}

int get_file_index(int square) {
    return square & 7;
}

int get_diagonal_index(int square) {
    return ((square >> 3) - (square & 7)) & 15;
}

int get_antidiagonal_index(int square) {
    return ((square >> 3) + (square & 7)) ^ 7;
}
