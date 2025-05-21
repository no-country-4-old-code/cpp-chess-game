#include <iostream>
#include "run-game.h"
#include "player.h"

/**
 * @file main.cpp
 * @brief Main entry point for the program, prints the project information.
 *
 * This function outputs the project name, version, and commit hash to the
 * console. It serves as the starting point for executing the program.
 *
 */
int main() {
    board::Board board{8, 8};

    using namespace piece;
    using namespace board::notation::literal;

    piece::army::army_list army_list = {
        piece::army::Army{Color::BLUE, {
                                           King{"a1"_n.as_squares(board)},
                                           Rock{"e4"_n.as_squares(board)},
                                           Rock{"b4"_n.as_squares(board)},
                                           Bishop{board, "b6"_n},
                                       }},
        piece::army::Army{Color::WHITE, {
                                            King{"h1"_n.as_squares(board)},
                                            Rock{"f8"_n.as_squares(board)},
                                            Rock{"g7"_n.as_squares(board)},
                                            Rock{"h6"_n.as_squares(board)},
                                        }},
        piece::army::Army{Color::ORANGE, {King{"d8"_n.as_squares(board)}, Rock{"d7"_n.as_squares(board)}}}, piece::army::Army{}};

    piece::api::init_army_list(army_list, board);

    ChessAI a{army_list, board};
    demo::game::PlayerGroup group{
        Player{&a, &army_list[0]},
        Player{&a, &army_list[1]},
        Player{&a, &army_list[2]},
    };

    demo::game::run(group, board, army_list);
}