//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <string.h>
#include <stdio.h>

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

Move* string_to_move(const char* move, Piece** mailbox) {
    char* start_file_str = malloc(sizeof(char));
    char* end_file_str = malloc(sizeof(char));
    char* promotion_str = malloc(sizeof(char));
    start_file_str[0] = move[0];
    end_file_str[0] = move[2];
    promotion_str[0] = strlen(move) == 5 ? move[4] : '\0';

    int start_file = (int)strcspn(FILES, start_file_str);
    int start_rank = move[1] - '0';
    int end_file = (int)strcspn(FILES, end_file_str);
    int end_rank = move[3] - '0';
    int promotion = (int)strcspn(PIECES, promotion_str) + 1;
    if (promotion == 7) promotion = 0;

    int start = get_square_index(start_file, (8 - start_rank));
    int end = get_square_index(end_file, (8 - end_rank));

    if (mailbox[start]->pieceType == PieceType_PAWN && start_file != end_file && mailbox[end]->pieceType == PieceType_NONE)
        promotion = PieceType_PAWN;
    if (mailbox[start]->pieceType == PieceType_KING && (strcmp(move, "e1c1") == 0 || strcmp(move, "e1g1") == 0 || strcmp(move, "e8c8") == 0 || strcmp(move, "e8g8") == 0))
        promotion = PieceType_KING;

    return move_new(start, end, mailbox[start]->pieceType, mailbox[end]->pieceType, promotion);
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