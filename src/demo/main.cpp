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
 * @brief Demo to run a custom chess board game.
 *
 */
int main() {
    using namespace piece;                     // NOLINT - accepted in top-level function
    using namespace board::notation::literal;  // NOLINT - using namespace for literals is pretty ok

    board::Board const board{4, 9};
    const auto up = MoveDirection::UP;
    const auto down = MoveDirection::DOWN;

    // TODO: Need to add "repetion detection to prevent endless game" - maybe with hashmap with overall attack-map as seed ?

    army::army_list army_list = {
        army::Army{Color::WHITE, {
            Rock(board, "a1"_n), Bishop(board, "b1"_n), King(board, "c1"_n), Knight(board, "d1"_n),
            Pawn(board, "a2"_n, down), Pawn(board, "b2"_n, down), Pawn(board, "c2"_n, down), Pawn(board, "d2"_n, down) 
        }},
        army::Army{Color::BLACK, {
            Pawn(board, "a7"_n, up), Pawn(board, "b7"_n, up), Pawn(board, "c7"_n, up), Pawn(board, "d7"_n, up), 
            Pawn(board, "a8"_n, up), Pawn(board, "b8"_n, up), King(board, "c8"_n), Pawn(board, "d8"_n, up)
        }},army::Army{}, army::Army{}};

    ai::ChessAI ai{army_list, board};

    demo::game::PlayerGroup group{
        Player{&ai, &army_list[0]},
        Player{&ai, &army_list[1]}
    };

    demo::game::run(group, board, army_list);
}