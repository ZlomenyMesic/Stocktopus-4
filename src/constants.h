//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#ifndef STOCKTOPUS_4_CONSTANTS_H
#define STOCKTOPUS_4_CONSTANTS_H

#define ENGINE_INFO "id name Stocktopus 4\nid author ZlomenyMesic\nuciok\n"

#define MAX_INPUT_LENGTH 72000
#define MAX_TOKEN_COUNT 1000
#define TOKEN_SIZE 72

#define STARTPOS_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define PIECES "pnbrqk"
#define FILES "abcdefgh"

typedef unsigned long long ulong;

typedef enum {
    PieceType_NONE = 0,
    PieceType_PAWN = 1,
    PieceType_KNIGHT = 2,
    PieceType_BISHOP = 3,
    PieceType_ROOK = 4,
    PieceType_QUEEN = 5,
    PieceType_KING = 6
} PieceType;

typedef enum {
    Color_WHITE = 0,
    Color_BLACK = 1,
    Color_NONE = 2
} Color;

#endif
