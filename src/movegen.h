//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_MOVEGEN_H
#define STOCKTOPUS_4_MOVEGEN_H

#include "constants.h"
#include "board.h"

void init_lookup_tables();

void get_pawn_moves(const Board* board, ulong pawn, Move** moves, int* index);
void get_knight_moves(const Board* board, ulong knight, Move** moves, int* index);
void get_bishop_moves(const Board* board, ulong bishop, Move** moves, int* index);
void get_rook_moves(const Board* board, ulong rook, Move** moves, int* index);
void get_king_moves(const Board* board, ulong king, Move** moves, int* index);
void get_castling_moves(const Board* board, Move** moves, int* index);

int get_legal_moves(Move** array, const Board* board, Color color);

#endif
