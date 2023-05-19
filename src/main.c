//
// Stocktopus-4 chess engine
//      by ZlomenyMesic
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "game.h"
#include "movegen.h"

void run_command(const char** tokens, int tokc, int skip) {
    if (strcmp(tokens[0 + skip], "uci") == 0) printf(ENGINE_INFO);
    else if (strcmp(tokens[0 + skip], "isready") == 0) printf("readyok\n");
    else if (strcmp(tokens[0 + skip], "position") == 0) set_position(tokens, tokc, skip);
    else if (strcmp(tokens[0 + skip], "go") == 0) best_move();
}

void process_command(char* command) {
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

    run_command(tokens, tok_count, 0);

    for (int i = 0; i < MAX_TOKEN_COUNT; i++) {
        free(tokens[i]);
    }
    free(tokens);
    free(current_token);
}

int main(int argc, char** args) {
    init_moves();
    if (argc == 1) {
        for (;;) {
            char console_input[MAX_INPUT_LENGTH];
            gets(console_input);
            process_command(console_input);
        }
    } else {
        char** cur_cmd = malloc(MAX_TOKEN_COUNT * TOKEN_SIZE);
        int cur_cmdc = 0;
        for (int i = 1; i < argc; i++) {
            if (strcmp(args[i], "&") == 0) {
                run_command(cur_cmd, cur_cmdc, 0);
                for (int i = 0; i < cur_cmdc; i++) {
                    free(cur_cmd[i]);
                }
                cur_cmdc = 0;
            } else cur_cmd[cur_cmdc++] = args[i];
        }
        run_command(cur_cmd, cur_cmdc, 0);
        for (int i = 0; i < cur_cmdc; i++) {
            free(cur_cmd[i]);
        }
        free(cur_cmd);
    }
}
