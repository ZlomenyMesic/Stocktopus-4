//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <ctype.h>
#include "piece.h"

Piece* piece_new(Color color, PieceType type) {
    Piece* piece = malloc(sizeof(Piece));
    piece->color = color;
    piece->pieceType = type;
    return piece;
}

void piece_destruct(Piece* piece) {
    free(piece);
}

char piece_to_char(const Piece* piece) {
    if (piece->color == Color_NONE || piece->pieceType == PieceType_NONE) return '-';
    char c = PIECES[piece->pieceType - 1];
    return (char)(piece->color == Color_BLACK ? c : toupper(c));
}