//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <stdio.h>
#include <string.h>
#include "move.h"
#include "movegen.h"

Move* move_new(int start, int end, PieceType piece, PieceType capture, PieceType promotion) {
    Move* move = malloc(sizeof(Move));
    move->start = start;
    move->end = end;
    move->piece = piece;
    move->capture = capture;
    move->promotion = promotion;
    return move;
}

char* move_to_string(const Move* move) {
    char* output = malloc(5 * sizeof(char));
    output[0] = FILES[get_file_index(move->start)];
    output[1] = (char)(56 - get_rank_index(move->start));
    output[2] = FILES[get_file_index(move->end)];
    output[3] = (char)(56 - get_rank_index(move->end));
    output[4] = move->promotion != PieceType_NONE && move->promotion != PieceType_KING && move->promotion != PieceType_PAWN
            ? PIECES[move->promotion - 1] : '\0';
    return output;
}