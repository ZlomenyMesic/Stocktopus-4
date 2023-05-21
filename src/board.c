//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <stdio.h>
#include <ctype.h>

#include "board.h"
#include "piece.h"
#include "bitboard.h"

Board* board_new() {
    Board* board = malloc(sizeof(Board));
    board->mailbox = malloc(64 * sizeof(Piece*));
    for (int i = 0; i < 64; i++) {
        board->mailbox[i] = piece_new(Color_NONE, PieceType_NONE);
    }
    board->pieces = malloc(2 * sizeof(ulong*));
    board->pieces[0] = malloc(6 * sizeof(ulong));
    board->pieces[1] = malloc(6 * sizeof(ulong));
    board->occupied = malloc(2 * sizeof(ulong));
    return board;
}

void board_destruct(Board* board) {
    for (int i = 0; i < 64; i++) {
        free(board->mailbox[i]);
    }
    free(board->mailbox);
    free(board->pieces[0]);
    free(board->pieces[1]);
    free(board->pieces);
    free(board->occupied);
    free(board);
}

void perform_move(Board* board, const Move* move) {
    Color color = board->mailbox[move->start]->color;
    Color op_color = color == Color_WHITE ? Color_BLACK : Color_WHITE;

    board->mailbox[move->end] = board->mailbox[move->start];
    board->mailbox[move->start] = piece_new(Color_NONE, PieceType_NONE);

    if (move->piece == PieceType_PAWN && ((color == Color_WHITE && move->start >= 48 && move->start <= 55 && move->end >= 32 && move->end <= 39)
         || (color == Color_BLACK && move->start <= 15 && move->start >= 8 && move->end >= 24 && move->end <= 31))) {
        board->en_passant_square = move->end;
    } else board->en_passant_square = 0;

    ulong from_bb = ((ulong)1 << move->start);
    ulong to_bb = ((ulong)1 << move->end);
    ulong from_to_bb = from_bb | to_bb;

    board->pieces[color][board->mailbox[move->end]->pieceType - 1] ^= from_to_bb;
    board->occupied[color] ^= from_to_bb;

    if (move->capture) {
        board->pieces[op_color][move->capture - 1] ^= to_bb;
        board->occupied[op_color] ^= to_bb;
        board->empty ^= from_bb;
    } board->empty ^= from_to_bb;

    if (move->promotion == PieceType_PAWN) {
        ulong en_passant_bb = color == Color_WHITE ? to_bb >> 8 : to_bb << 8;
        board->mailbox[move->end + (color == Color_WHITE ? 8 : -8)] = piece_new(Color_NONE, PieceType_NONE);
        board->pieces[op_color][0] ^= en_passant_bb;
        board->occupied[op_color] ^= en_passant_bb;
        board->empty ^= en_passant_bb;
    }
    else if (move->promotion == PieceType_KING) {
        switch (move->end) {
            case 2: perform_move(board, move_new(0, 3, 4, 0, 0)); break;
            case 6: perform_move(board, move_new(7, 5, 4, 0, 0)); break;
            case 58: perform_move(board, move_new(56, 59, 4, 0, 0)); break;
            case 62: perform_move(board, move_new(63, 61, 4, 0, 0)); break;
            default: printf("an error occurred while playing a castling move, move.end: %d\n", move->end);
        }
    }
    else if (move->promotion) {
        board->mailbox[move->end] = piece_new(color, move->promotion);
        board->pieces[color][0] ^= to_bb;
        board->pieces[color][move->promotion - 1] ^= to_bb;
    }

    update_bitboards(board);
}

int is_check(const Board* board, Color color) {
    return 0;
}

int is_move_legal(const Board* board, const Move* move) {
    return 1;
}

void board_print(const Board* board) {
    for (int i = 0; i < 64; i++) {
        char square = board->mailbox[i]->pieceType != PieceType_NONE
            ? PIECES[board->mailbox[i]->pieceType - 1] : '-';

        printf("%c ", board->mailbox[i]->color == Color_BLACK ? square : toupper(square));
        if ((i + 1) % 8 == 0) printf("\n");
    }
}

void update_bitboards(Board* board) {
    for (int i = 0; i < 6; i++) {
        board->pieces[0][i] = 0;
        board->pieces[1][i] = 0;
    }
    for (int i = 0; i < 64; i++) {
        if (board->mailbox[i]->pieceType != PieceType_NONE) {
            board->pieces[board->mailbox[i]->color][board->mailbox[i]->pieceType - 1] |= ((ulong)1 << i);
        }
    }

    board->occupied[0] = 0;
    board->occupied[1] = 0;
    for (int i = 0; i < 6; i++) {
        board->occupied[0] |= board->pieces[0][i];
        board->occupied[1] |= board->pieces[1][i];
    }
    board->empty = ~(board->occupied[0] | board->occupied[1]);
}