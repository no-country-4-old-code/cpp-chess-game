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

TEST(BoardMovementsHorizontal, Left_Valid_h8) {
    auto start        = "h8"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    expect_notation(end_square, "g8"_n);
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

TEST(BoardMovementsHorizontal, Right_Valid_a1) {
    auto start        = "a1"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    expect_notation(end_square, "b1"_n);
}

TEST(BoardMovementsHorizontal, Right_Valid_h1) {
    auto start        = "g1"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    expect_notation(end_square, "h1"_n);
}

TEST(BoardMovementsHorizontal, Right_Valid_b8) {
    auto start        = "b8"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    expect_notation(end_square, "c8"_n);
}

TEST(BoardMovementsHorizontal, Right_Invalid_h1) {
    auto start        = "h1"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsHorizontal, Right_Invalid_h8) {
    auto start        = "h8"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test right board movement
// Start -- Test up board movement

TEST(BoardMovementsVertical, Up_Valid_a2) {
    auto start        = "a2"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "a1"_n);
}

TEST(BoardMovementsVertical, Up_Valid_h2) {
    auto start        = "h2"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "h1"_n);
}

TEST(BoardMovementsVertical, Up_Valid_h8) {
    auto start        = "h8"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "h7"_n);
}

TEST(BoardMovementsVertical, Up_Invalid_a1) {
    auto start        = "a1"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsVertical, Up_Invalid_h1) {
    auto start        = "h1"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}