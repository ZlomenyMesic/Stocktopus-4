//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "game.h"
#include "bitboard.h"

void run_command(char* command) {
    char** tokens = malloc(MAX_TOKEN_COUNT * TOKEN_SIZE);
    char* current_token = strtok(command, " ");
    int tok_count = 0;

    for (int i = 0; i < MAX_TOKEN_COUNT; i++) {
        tokens[i] = malloc(TOKEN_SIZE);
    }

    while (current_token != NULL) {
        strcpy(tokens[tok_count++], current_token);
        current_token = strtok(NULL, " ");
    }

    if (strcmp(tokens[0], "uci") == 0) printf(ENGINE_INFO);
    else if (strcmp(tokens[0], "isready") == 0) printf("readyok\n");
    else if (strcmp(tokens[0], "position") == 0) set_position(tokens, tok_count);
    else if (strcmp(tokens[0], "go") == 0) printf("%s", best_move());

    for (int i = 0; i < MAX_TOKEN_COUNT; i++) {
        free(tokens[i]);
    }
    free(tokens);
    free(current_token);
}

int main() {
    for (;;) {
        char console_input[MAX_INPUT_LENGTH];
        gets(console_input);
        run_command(console_input);
    }
}
