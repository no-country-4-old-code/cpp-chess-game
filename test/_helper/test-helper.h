#pragma once
#include "army.h"
#include "board.h"

template <typename... Squares>
board::bitmap::Squares combine_squares(board::Board board, Squares... squares) {
    return (squares.as_squares(board) | ...);
}

namespace test::helper {
    void display_bits_on_board(const board::Board&, board::bitmap::Squares);
    piece::army::army_list create_army_list(board::Board, 
                                            const std::vector<piece::Piece>& white,
                                            const std::vector<piece::Piece>& black);
}
