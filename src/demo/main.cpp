#include "army.h"
#include "board.h"
#include "chess-ai.h"
#include "color.h"
#include "notation.h"
#include "pieces.h"
#include "player.h"
#include "run-game.h"

/**
 * @file main.cpp
 * @brief Main entry point for the program, prints the project information.
 *
 * This function outputs the project name, version, and commit hash to the
 * console. It serves as the starting point for executing the program.
 *
 */
int main() {
    using namespace piece; // NOLINT - accepted in top-level function
    using namespace board::notation::literal; //NOLINT - using namespace for literals is pretty ok

    board::Board const board{8, 8};

    army::army_list army_list = {
        army::Army{Color::BLUE, {King(board, "a1"_n), Rock(board, "e4"_n), Bishop(board, "b6"_n)}},
        army::Army{Color::WHITE,
                   {
                       King(board, "h2"_n),
                       Rock(board, "f8"_n),
                       Rock(board, "g7"_n),
                   }},
        army::Army{Color::ORANGE, {King(board, "d8"_n), Rock(board, "d7"_n)}}, army::Army{}};

    ChessAI ai{army_list, board};

    demo::game::PlayerGroup group{
        Player{&ai, &army_list[0]},
        Player{&ai, &army_list[1]},
        Player{&ai, &army_list[2]},
    };

    demo::game::run(group, board, army_list);
}