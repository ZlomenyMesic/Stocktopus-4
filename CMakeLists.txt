cmake_minimum_required(VERSION 3.25)
project(Stocktopus_4 C)

set(CMAKE_C_STANDARD 11)

add_executable(Stocktopus_3 src/main.c src/game.c src/board.c src/piece.c src/bitboard.c)
