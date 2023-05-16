//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <stdio.h>
#include <ctype.h>
#include "board.h"
#include "piece.h"

Board* board_new() {
    Board* board = malloc(sizeof(Board));
    board->mailbox = malloc(64 * sizeof(Piece*));
    for (int i = 0; i < 64; i++) {
        board->mailbox[i] = piece_new(Color_NONE, PieceType_NONE);
    }
    board->white_pieces = malloc(6 * sizeof(ulong));
    board->black_pieces = malloc(6 * sizeof(ulong));
    board->occupied = malloc(2 * sizeof(ulong));
    board->castling_flags = malloc(4 * sizeof(int));
    return board;
}

void board_destruct(Board* board) {
    for (int i = 0; i < 64; i++) {
        free(board->mailbox[i]);
    }
    free(board->mailbox);
    free(board->white_pieces);
    free(board->black_pieces);
    free(board->occupied);
    free(board->castling_flags);
    free(board);
}

void perform_move(Board* board, const Move* move) {

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
        board->white_pieces[i] = 0;
        board->black_pieces[i] = 0;
    }
    for (int i = 0; i < 64; i++) {
        if (board->mailbox[i]->color == Color_WHITE) {
            board->white_pieces[board->mailbox[i]->pieceType - 1] |= (1 << i);
        } else if (board->mailbox[i]->color == Color_BLACK) {
            board->black_pieces[board->mailbox[i]->pieceType - 1] |= (1 << i);
        }
    }

    board->occupied[0] = 0;
    board->occupied[1] = 0;
    for (int i = 0; i < 6; i++) {
        board->occupied[0] |= board->white_pieces[i];
        board->occupied[1] |= board->black_pieces[i];
    }
    board->empty = ~(board->occupied[0] | board->occupied[1]);
}