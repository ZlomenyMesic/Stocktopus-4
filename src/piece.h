//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_PIECE_H
#define STOCKTOPUS_4_PIECE_H

#include <stdlib.h>
#include "constants.h"

typedef struct {
    Color color;
    PieceType pieceType;
} Piece;

Piece* piece_new(Color color, PieceType type);
void piece_destruct(Piece* piece);
char piece_to_char(const Piece* piece);

#endif
