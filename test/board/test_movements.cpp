#include <board-movements.h>
#include <gtest/gtest.h>
#include <notation.h>
#include <squares.h>
#include <sstream>
#include <tuple>
#include "board-movements.h"

using namespace board::notation::literal;
namespace move = board::movements;
namespace notation = board::notation;

namespace {
    const board::Board default_board{8, 8};

    void expect_notation(board::bitmap::Squares position, notation::ChessNotation expected) {
        notation::ChessNotation end{position, default_board};
        EXPECT_EQ(end, expected);
    }
}

// Start -- Test left board movement

TEST(BoardMovementsHorizontal, Left_Valid_b1) {
    auto start        = "b1"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    expect_notation(end_square, "a1"_n);
}

TEST(BoardMovementsHorizontal, Left_Valid_g8) {
    auto start        = "g8"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    expect_notation(end_square, "f8"_n);
}

TEST(BoardMovementsHorizontal, Left_Invalid_a1) {
    auto start        = "a1"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsHorizontal, Left_Invalid_a2) {
    auto start        = "a2"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsHorizontal, Left_Invalid_a8) {
    auto start        = "a8"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test left board movement
// Start -- Test right board movement
