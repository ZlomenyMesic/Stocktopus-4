//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_MOVEGEN_H
#define STOCKTOPUS_4_MOVEGEN_H

#include "constants.h"
#include "board.h"

void init_moves();

void init_pawn_moves();
void init_knight_moves();

int get_legal_moves(Move** moves, const Board* board, Color color);

int get_pawn_moves(const Board* board, Color color, Move** moves, int index);
int get_knight_moves(const Board* board, Color color, Move** moves, int index);
int get_bishop_moves(const Board* board, Color color, Move** moves, int index);
int get_rook_moves(const Board* board, Color color, Move** moves, int index);
int get_king_moves(const Board* board, Color color, Move** moves, int index);
int get_castling_moves(const Board* board, Color color, Move** moves, int index);

ulong get_pawn_targets(ulong pawn, const Board* board, Color color);
ulong get_knight_targets(ulong knight, const Board* board, Color color);
ulong get_bishop_targets(ulong bishop, const Board* board, Color color);
ulong get_rook_targets(ulong rook, const Board* board, Color color);
ulong get_king_targets(ulong king, const Board* board, Color color);

int get_square_index(int file, int rank);
int get_rank_index(int square);
int get_file_index(int square);
int get_diagonal_index(int square);
int get_antidiagonal_index(int square);

#endif
