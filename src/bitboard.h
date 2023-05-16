//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_BITBOARD_H
#define STOCKTOPUS_4_BITBOARD_H

#include "constants.h"

int is_bit_set(ulong bitboard, int index);
int bit_scan_forward(ulong bitboard);
int bit_scan_forward_reset(ulong* bitboard);
int popcount(ulong bitboard);

#endif
