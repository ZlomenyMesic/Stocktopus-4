//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "game.h"

Board* chessboard;

void set_position(const char** tokens, int tok_count) {
    chessboard = board_new();
    if (tok_count > 1 && strcmp(tokens[1], "startpos") == 0) {
        engineColor = Color_WHITE;
        playerColor = Color_BLACK;
        set_position_fen(STARTPOS_FEN);
    } else if (strcmp(tokens[1], "fen") == 0) {
        if (tok_count < 8) {
            printf("missing parameters in: position fen\n");
            return;
        }
        char* fen = tokens[2];
        for (int i = 3; i < 8; i++) {
            strcat(fen, " ");
            strcat(fen, tokens[i]);
        }
        set_position_fen(fen);
    } else if (tok_count == 1) printf("missing parameters in: position\n");
    else printf("unknown token: %s\n", tokens[1]);
}

void set_position_fen(const char* fen) {
    board_destruct(chessboard);
    chessboard = board_new();

    char fencpy[strlen(fen)];
    strcpy(fencpy, fen);

    char** tokens = malloc(6 * 72);
    char* current_token = strtok(fencpy, " ");
    int tok_count = 0;

    for (int i = 0; i < 6; i++) {
        tokens[i] = malloc(72);
    }

    while (current_token != NULL) {
        strcpy(tokens[tok_count++], current_token);
        current_token = strtok(NULL, " ");
    }

    for (int i = 0, square = 0; i < strlen(tokens[0]); i++) {
        int isvalid = 0;
        if (square > 63) {
            printf("too many squares in fen: %s\n", tokens[0]);
            break;
        }
        if (isdigit(tokens[0][i])) {
            square += tokens[0][i] - '0';
            isvalid = 1;
        } else if (isalpha(tokens[0][i])) {
            for (int j = 0; j < 6; j++) {
                if (PIECES[j] == tolower(tokens[0][i])) {
                    chessboard->mailbox[square]->pieceType = (PieceType) (j + 1);
                    chessboard->mailbox[square++]->color = isupper(tokens[0][i]) ? Color_WHITE : Color_BLACK;
                    isvalid = 1;
                }
            }
        } else if (tokens[0][i] == '/') isvalid = 1;
        if (!isvalid) printf("invalid or unsupported character: %c\n", tokens[0][i]);
    }

    if (strcmp(tokens[1], "w") == 0) {
        engineColor = Color_WHITE;
        playerColor = Color_BLACK;
    } else if (strcmp(tokens[1], "b") == 0) {
        engineColor = Color_BLACK;
        playerColor = Color_WHITE;
    } else printf("invalid side to move: %s\n", tokens[1]);

    for (int i = 0; i < strlen(tokens[2]); i++) {
        switch (tokens[2][i]) {
            case 'K': chessboard->castling_flags[0] = 1; break;
            case 'Q': chessboard->castling_flags[1] = 1; break;
            case 'k': chessboard->castling_flags[2] = 1; break;
            case 'q': chessboard->castling_flags[3] = 1; break;
            default: if (tokens[2][i] != '-') printf("invalid castling availability: %c\n", tokens[2][i]); break;
        }
    }

    chessboard->en_passant_square = -1;
    if (strlen(tokens[3]) == 2 && isdigit(tokens[3][0]) && isdigit(tokens[3][1])) {
        chessboard->en_passant_square = atoi(tokens[3]);
    } else if (strcmp(tokens[3], "-") != 0) printf("invalid en passant square: %s\n", tokens[3]);

    update_bitboards(chessboard);
}

char* best_move() {
    return "bestmove e7e5\n";
}

void new_game() {

}