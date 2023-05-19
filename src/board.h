//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_BOARD_H
#define STOCKTOPUS_4_BOARD_H

#include "constants.h"
#include "piece.h"
#include "move.h"

typedef struct {
    ulong** pieces;

    ulong* occupied;
    ulong empty;

    Piece** mailbox;

    int* castling_flags;
    int en_passant_square;
    int number_of_pieces;
} Board;

Board* board_new();
void board_destruct(Board* board);

void perform_move(Board* board, const Move* move);
void board_print(const Board* board);
int is_check(const Board* board, Color color);
int is_move_legal(const Board* board, const Move* move);

void update_bitboards(Board* board);

#endif
