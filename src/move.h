//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_MOVE_H
#define STOCKTOPUS_4_MOVE_H

#include "piece.h"

typedef struct {
    int start, end;

    PieceType piece;
    PieceType capture;
    PieceType promotion;
} Move;

Move* move_new(int start, int end, PieceType piece, PieceType capture, PieceType promotion);
void move_destruct(Move* move);

Move* string_to_move(char* move, Piece** mailbox);
char* move_to_string(const Move* move);

#endif
