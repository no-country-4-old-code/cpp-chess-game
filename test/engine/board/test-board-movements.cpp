#include <board-movements.h>
#include <gtest/gtest.h>
#include <notation.h>
#include <squares.h>
#include <sstream>
#include <tuple>
#include "board-movements.h"

using namespace board::notation::literal;
namespace move     = board::movements;
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
    auto start      = "b1"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    expect_notation(end_square, "a1"_n);
}

TEST(BoardMovementsHorizontal, Left_Valid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    expect_notation(end_square, "g8"_n);
}

TEST(BoardMovementsHorizontal, Left_Invalid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsHorizontal, Left_Invalid_a2) {
    auto start      = "a2"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsHorizontal, Left_Invalid_a8) {
    auto start      = "a8"_n;
    auto end_square = move::left(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test left board movement
// Start -- Test right board movement

TEST(BoardMovementsHorizontal, Right_Valid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    expect_notation(end_square, "b1"_n);
}

TEST(BoardMovementsHorizontal, Right_Valid_h1) {
    auto start      = "g1"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    expect_notation(end_square, "h1"_n);
}

TEST(BoardMovementsHorizontal, Right_Valid_b8) {
    auto start      = "b8"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    expect_notation(end_square, "c8"_n);
}

TEST(BoardMovementsHorizontal, Right_Invalid_h1) {
    auto start      = "h1"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsHorizontal, Right_Invalid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::right(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsHorizontal, Right_Invalid_d2_small_board) {
    board::Board board{4, 4};
    auto start      = "d2"_n;
    auto end_square = move::right(start.as_squares(board), board);
    EXPECT_EQ(end_square, 0);
}


// End -- Test right board movement
// Start -- Test up board movement

TEST(BoardMovementsVertical, Up_Valid_a2) {
    auto start      = "a2"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "a1"_n);
}

TEST(BoardMovementsVertical, Up_Valid_h2) {
    auto start      = "h2"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "h1"_n);
}

TEST(BoardMovementsVertical, Up_Valid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "h7"_n);
}

TEST(BoardMovementsVertical, Up_Invalid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsVertical, Up_Invalid_h1) {
    auto start      = "h1"_n;
    auto end_square = move::up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test up board movement
// Start -- Test down board movement

TEST(BoardMovementsVertical, Down_Valid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::down(start.as_squares(default_board), default_board);
    expect_notation(end_square, "a2"_n);
}

TEST(BoardMovementsVertical, Down_Valid_d4) {
    auto start      = "d4"_n;
    auto end_square = move::down(start.as_squares(default_board), default_board);
    expect_notation(end_square, "d5"_n);
}

TEST(BoardMovementsVertical, Down_Valid_h1) {
    auto start      = "h1"_n;
    auto end_square = move::down(start.as_squares(default_board), default_board);
    expect_notation(end_square, "h2"_n);
}

TEST(BoardMovementsVertical, Down_Invalid_h1) {
    auto start      = "a8"_n;
    auto end_square = move::down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsVertical, Down_Invalid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsVertical, Down_Invalid_c1_small_board) {
    board::Board board{4, 4};
    auto start      = "a4"_n;
    auto end_square = move::down(start.as_squares(board), board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test down board movement

// === Diagonal Movement

// Start -- Test left-up board movement

TEST(BoardMovementsDiagonal, Left_Up_Valid_b2) {
    auto start      = "b2"_n;
    auto end_square = move::left_up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "a1"_n);
}

TEST(BoardMovementsDiagonal, Left_Up_Valid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::left_up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "g7"_n);
}

TEST(BoardMovementsDiagonal, Left_Up_Invalid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::left_up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Left_Up_Invalid_a8) {
    auto start      = "a8"_n;
    auto end_square = move::left_up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Left_Up_Invalid_h1) {
    auto start      = "h1"_n;
    auto end_square = move::left_up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test left-up board movement
// Start -- Test left-down board movement

TEST(BoardMovementsDiagonal, Left_Down_Valid_b2) {
    auto start      = "b2"_n;
    auto end_square = move::left_down(start.as_squares(default_board), default_board);
    expect_notation(end_square, "a3"_n);
}

TEST(BoardMovementsDiagonal, Left_Down_Valid_h1) {
    auto start      = "h1"_n;
    auto end_square = move::left_down(start.as_squares(default_board), default_board);
    expect_notation(end_square, "g2"_n);
}

TEST(BoardMovementsDiagonal, Left_Down_Invalid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::left_down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Left_Down_Invalid_a8) {
    auto start      = "a8"_n;
    auto end_square = move::left_down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Left_Down_Invalid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::left_down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test left-down board movement
// Start -- Test right-up board movement

TEST(BoardMovementsDiagonal, Right_Up_Valid_b2) {
    auto start      = "b2"_n;
    auto end_square = move::right_up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "c1"_n);
}

TEST(BoardMovementsDiagonal, Right_Up_Valid_a8) {
    auto start      = "a8"_n;
    auto end_square = move::right_up(start.as_squares(default_board), default_board);
    expect_notation(end_square, "b7"_n);
}

TEST(BoardMovementsDiagonal, Right_Up_Invalid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::right_up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Right_Up_Invalid_h1) {
    auto start      = "h1"_n;
    auto end_square = move::right_up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Right_Up_Invalid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::right_up(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test right-up board movement
// Start -- Test right-down board movement

TEST(BoardMovementsDiagonal, Right_Down_Valid_a1) {
    auto start      = "a1"_n;
    auto end_square = move::right_down(start.as_squares(default_board), default_board);
    expect_notation(end_square, "b2"_n);
}

TEST(BoardMovementsDiagonal, Right_Down_Valid_d4) {
    auto start      = "d4"_n;
    auto end_square = move::right_down(start.as_squares(default_board), default_board);
    expect_notation(end_square, "e5"_n);
}

TEST(BoardMovementsDiagonal, Right_Down_Invalid_h1) {
    auto start      = "h1"_n;
    auto end_square = move::right_down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Right_Down_Invalid_h8) {
    auto start      = "h8"_n;
    auto end_square = move::right_down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

TEST(BoardMovementsDiagonal, Right_Down_Invalid_a8) {
    auto start      = "a8"_n;
    auto end_square = move::right_down(start.as_squares(default_board), default_board);
    EXPECT_EQ(end_square, 0);
}

// End -- Test right-down board movement
