//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_GAME_H
#define STOCKTOPUS_4_GAME_H

#include "board.h"

static Color engineColor;
static Color playerColor;

void set_position(const char** tokens, int tok_count, int skip);
void set_position_fen(const char* fen);
void best_move();
void set_option(const char** tokens, int tok_count, int skip);
void new_game();

#endif
