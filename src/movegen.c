//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include "movegen.h"
#include "bitboard.h"
#include "constants.h"
#include "move.h"

ulong white_pawn_spushes[64];
ulong black_pawn_spushes[64];
ulong white_pawn_dpushes[64];
ulong black_pawn_dpushes[64];
ulong white_pawn_attacks[64];
ulong black_pawn_attacks[64];
ulong knight_attacks[64];
ulong king_attacks[64];
ulong rank_attacks[64][64];
ulong file_attacks[64][64];
ulong diagonal_attacks[64][64];
ulong antidiagonal_attacks[64][64];

void init_moves() {
    init_pawn_moves();
    init_knight_attacks();
    init_king_attacks();
    init_rank_attacks();
    init_file_attacks();
    init_diagonal_attacks();
    init_antidiagonal_attacks();
}

void init_pawn_moves() {
    for (int square = 0; square < 64; square++) {
        if (square < 8 || square > 55) {
            white_pawn_spushes[square] = 0;
            black_pawn_spushes[square] = 0;
            white_pawn_dpushes[square] = 0;
            black_pawn_dpushes[square] = 0;
            white_pawn_attacks[square] = 0;
            black_pawn_attacks[square] = 0;
        }
        else {
            ulong w_single_step = (ulong)1 << (square - 8);
            ulong b_single_step = (ulong)1 << (square + 8);

            ulong w_double_step = w_single_step >> 8 & 0x000000FF00000000;
            ulong b_double_step = b_single_step << 8 & 0x00000000FF000000;

            ulong w_west_attacks = (ulong)1 << (square - 7) & 0xFEFEFEFEFEFEFEFE;
            ulong b_west_attacks = (ulong)1 << (square + 9) & 0xFEFEFEFEFEFEFEFE;

            ulong w_east_attacks = (ulong)1 << (square - 9) & 0x7F7F7F7F7F7F7F7F;
            ulong b_east_attacks = (ulong)1 << (square + 7) & 0x7F7F7F7F7F7F7F7F;

            white_pawn_spushes[square] = w_single_step;
            black_pawn_spushes[square] = b_single_step;
            white_pawn_dpushes[square] = w_double_step;
            black_pawn_dpushes[square] = b_double_step;
            white_pawn_attacks[square] = w_west_attacks | w_east_attacks;
            black_pawn_attacks[square] = b_west_attacks | b_east_attacks;
        }
    }
}

void init_knight_attacks() {
    for (int square = 0; square < 64; square++) {
        ulong knight = (ulong)1 << square;
        ulong east = knight >> 1 & 0x7F7F7F7F7F7F7F7F;
        ulong west = knight << 1 & 0xFEFEFEFEFEFEFEFE;
        ulong targets = ((east | west) << 16) | ((east | west) >> 16);

        east = east >> 1 & 0x7F7F7F7F7F7F7F7F;
        west = west << 1 & 0xFEFEFEFEFEFEFEFE;
        knight_attacks[square] = targets | ((east | west) << 8) | ((east | west) >> 8);
    }
}

void init_rank_attacks() {
    for (int square = 0; square < 64; square++) {
        for (int occupancy = 0; occupancy < 64; occupancy++) {
            ulong occupancy_bb = (ulong)occupancy << 1;
            ulong targets = 0;

            int blocker = get_file_index(square) + 1;
            while (blocker <= 7) {
                targets |= (ulong)1 << blocker;
                if (is_bit_set(occupancy_bb, blocker++)) break;
            }

            blocker = get_file_index(square) - 1;
            while (blocker >= 0) {
                targets |= (ulong)1 << blocker;
                if (is_bit_set(occupancy_bb, blocker--)) break;
            }
            rank_attacks[square][occupancy] = targets << (get_rank_index(square) * 8);
        }
    }
}

void init_file_attacks() {
    for (int square = 0; square < 64; square++) {
        for (int occupancy = 0; occupancy < 64; occupancy++) {
            ulong rank_targets = rank_attacks[7 - (square >> 3)][occupancy];
            ulong targets = 0;

            for (int bit = 0; bit < 8; bit++) {
                if (is_bit_set(rank_targets, bit)) {
                    targets |= (ulong)1 << (get_file_index(square) + 8 * (7 - bit));
                }
            }
            file_attacks[square][occupancy] = targets;
        }
    }
}

void init_diagonal_attacks() {
    for (int square = 0; square < 64; square++) {
        for (int occupancy = 0; occupancy < 64; occupancy++) {
            int diagonal = get_rank_index(square) - get_file_index(square);
            ulong targets = 0;
            ulong rank_targets = diagonal > 0
                    ? rank_attacks[get_file_index(square)][occupancy]
                    : rank_attacks[get_rank_index(square)][occupancy];

            for (int bit = 0; bit < 8; bit++) {
                int rank, file;
                if (is_bit_set(rank_targets, bit)) {
                    if (diagonal >= 0) {
                        rank = diagonal + bit;
                        file = bit;
                    } else {
                        file = bit - diagonal;
                        rank = bit;
                    }
                    if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
                        targets |= (ulong)1 << get_square_index(file, rank);
                    }
                }
            }
            diagonal_attacks[square][occupancy] = targets;
        }
    }
}

void init_antidiagonal_attacks() {
    for (int square = 0; square < 64; square++) {
        for (int occupancy = 0; occupancy < 64; occupancy++) {
            int diagonal = get_diagonal_index(square);
            ulong targets = 0;
            ulong rank_targets = diagonal > 7
                    ? rank_attacks[7 - get_rank_index(square)][occupancy]
                    : rank_attacks[get_file_index(square)][occupancy];

            for (int bit = 0; bit < 8; bit++) {
                int rank, file;
                if (is_bit_set(rank_targets, bit)) {
                    if (diagonal >= 7) {
                        rank = 7 - bit;
                        file = diagonal - 7 + bit;
                    } else {
                        rank = diagonal - bit;
                        file = bit;
                    }
                    if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
                        targets |= (ulong)1 << get_square_index(file, rank);
                    }
                }
            }
            antidiagonal_attacks[square][occupancy] = targets;
        }
    }
}

void init_king_attacks() {
    for (int square = 0; square < 64; square++) {
        ulong king = (ulong)1 << square;
        king |= ((king << 1) & 0x7F7F7F7F7F7F7F7F) | ((king >> 1) & 0xFEFEFEFEFEFEFEFE);
        king |= (king << 8) | (king >> 8);
        king_attacks[square] = king ^ ((ulong)1 << square);
    }
}

int get_legal_moves(Move** moves, const Board* board, Color color) {
    int move_count = 0;
    move_count = get_pawn_moves(board, color, moves, move_count);
    move_count = get_knight_moves(board, color, moves, move_count);
    move_count = get_king_moves(board, color, moves, move_count);
    move_count = get_bishop_moves(board, color, moves, move_count);
    move_count = get_rook_moves(board, color, moves, move_count);
    move_count = get_queen_moves(board, color, moves, move_count);
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
            } else moves[index++] = move_new(start, end, PieceType_PAWN, board->mailbox[end]->pieceType, 0);
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
            moves[index++] = move_new(start, end, PieceType_KNIGHT, board->mailbox[end]->pieceType, 0);
        }
    }
    return index;
}

int get_king_moves(const Board* board, Color color, Move** moves, int index) {
    int start = bit_scan_forward(board->pieces[color][5]);
    ulong targets = get_king_targets(board->pieces[color][5], board, color);
    while (targets) {
        int end = bit_scan_forward_reset(&targets);
        moves[index++] = move_new(start, end, PieceType_KING, board->mailbox[end]->pieceType, 0);
    }
    return index;
}

int get_bishop_moves(const Board* board, Color color, Move** moves, int index) {
    ulong bishops = board->pieces[color][2];
    while (bishops) {
        int start = bit_scan_forward_reset(&bishops);
        ulong targets = get_bishop_targets((ulong)1 << start, board, color);
        while (targets) {
            int end = bit_scan_forward_reset(&targets);
            moves[index++] = move_new(start, end, PieceType_BISHOP, board->mailbox[end]->pieceType, 0);
        }
    }
    return index;
}

int get_rook_moves(const Board* board, Color color, Move** moves, int index) {
    ulong rooks = board->pieces[color][3];
    while (rooks) {
        int start = bit_scan_forward_reset(&rooks);
        ulong targets = get_rook_targets((ulong)1 << start, board, color);
        while (targets) {
            int end = bit_scan_forward_reset(&targets);
            moves[index++] = move_new(start, end, PieceType_ROOK, board->mailbox[end]->pieceType, 0);
        }
    }
    return index;
}

int get_queen_moves(const Board* board, Color color, Move** moves, int index) {
    ulong queens = board->pieces[color][4];
    while (queens) {
        int start = bit_scan_forward_reset(&queens);
        ulong targets = get_rook_targets((ulong)1 << start, board, color) | get_bishop_targets((ulong)1 << start, board, color);
        while (targets) {
            int end = bit_scan_forward_reset(&targets);
            moves[index++] = move_new(start, end, PieceType_QUEEN, board->mailbox[end]->pieceType, 0);
        }
    }
    return index;
}

int get_castling_moves(const Board* board, Color color, Move** moves, int index) {
    return index;
}

ulong get_pawn_targets(ulong pawn, const Board* board, Color color) {
    int index = bit_scan_forward(pawn);
    ulong spush_targets = (color == Color_WHITE ? white_pawn_spushes[index] : black_pawn_spushes[index]) & board->empty;
    ulong dpush_targets = spush_targets == 0 ? 0 : (color == Color_WHITE ? white_pawn_dpushes[index] : black_pawn_dpushes[index]) & board->empty;
    ulong atk_targets = color == Color_WHITE ? white_pawn_attacks[index] : black_pawn_attacks[index];
    return spush_targets | dpush_targets | (atk_targets & board->occupied[color == Color_WHITE ? 1 : 0]);
}

ulong get_knight_targets(ulong knight, const Board* board, Color color) {
    return knight_attacks[bit_scan_forward(knight)] & (board->empty | board->occupied[color == Color_WHITE ? 1 : 0]);
}

ulong get_king_targets(ulong king, const Board* board, Color color) {
    return king_attacks[bit_scan_forward(king)] & (board->empty & board->occupied[color == Color_WHITE ? 1 : 0]);
}

ulong get_bishop_targets(ulong bishop, const Board* board, Color color) {
    ulong occupied = ~board->empty;
    int square = bit_scan_forward(bishop);

    int diagonal = 7 + get_rank_index(square) - get_file_index(square);
    int occupancy = (int)((occupied & diagonal_mask[diagonal]) * diagonal_magic[diagonal] >> 56);
    ulong targets = diagonal_attacks[square][(occupancy >> 1) & 63];

    diagonal = get_file_index(square) + get_rank_index(square);
    occupancy = (int)((occupied & antidiagonal_mask[diagonal]) * antidiagonal_magic[diagonal] >> 56);
    targets |= antidiagonal_attacks[square][(occupancy >> 1) & 63];

    return targets & (board->empty | board->occupied[color == Color_WHITE ? 1 : 0]);
}

ulong get_rook_targets(ulong rook, const Board* board, Color color) {
    ulong occupied = ~board->empty;
    int square = bit_scan_forward(rook);

    int occupancy = (int)((occupied & six_bit_rank_mask[get_rank_index(square)]) >> (8 * get_rank_index(square)));
    ulong targets = rank_attacks[square][(occupancy >> 1) & 63];

    occupancy = (int)((occupied & six_bit_file_mask[get_file_index(square)]) * file_magic[get_file_index(square)] >> 56);
    targets |= file_attacks[square][(occupancy >> 1) & 63];

    return targets & (board->empty | board->occupied[color == Color_WHITE ? 1 : 0]);
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
    return (square >> 3) + (square & 7);
}