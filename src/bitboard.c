//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include "bitboard.h"

int is_bit_set(ulong bitboard, int index) {
    return (bitboard & ((ulong)1 << index)) != 0;
}

int bit_scan_forward(ulong bitboard) {
    return __builtin_ffsll((long long)bitboard) - 1;
}

int bit_scan_forward_reset(ulong* bitboard) {
    int index = __builtin_ffsll((long long)*bitboard) - 1;
    *bitboard ^= ((ulong)1 << index);
    return index;
}

int popcount(ulong bitboard) {
    return __builtin_popcountll(bitboard);
}