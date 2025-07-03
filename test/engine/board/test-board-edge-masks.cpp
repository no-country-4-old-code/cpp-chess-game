#include <gtest/gtest.h>
#include <notation.h>
#include <squares.h>
#include <sstream>
#include <tuple>
#include "board-edges.h"
#include "test-helper.h"

using namespace board::notation::literal;
using namespace board::edges;


TEST(BoardEdgeMaskTop, default_8x8) {
    board::Board board{8, 8};
    auto edge_mask = build_top_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "b1"_n, "c1"_n, "d1"_n, "e1"_n, "f1"_n, "g1"_n, "h1"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskTop, custom_4x8) {
    board::Board board{4, 8};
    auto edge_mask = build_top_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "b1"_n, "c1"_n, "d1"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskTop, custom_2x8) {
    board::Board board{2, 8};
    auto edge_mask = build_top_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "b1"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskTop, custom_10x4) {
    board::Board board{10, 4};
    auto edge_mask = build_top_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "b1"_n, "c1"_n, "d1"_n, "e1"_n, "f1"_n, "g1"_n, "h1"_n, "i1"_n, "j1"_n);
    EXPECT_EQ(edge_mask, expected);
}

// Left

TEST(BoardEdgeMaskLeft, default_8x8) {
    board::Board board{8, 8};
    auto edge_mask = build_left_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "a2"_n, "a3"_n, "a4"_n, "a5"_n, "a6"_n, "a7"_n, "a8"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskLeft, custom_3x5) {
    board::Board board{3, 5};
    auto edge_mask = build_left_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "a2"_n, "a3"_n, "a4"_n, "a5"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskLeft, custom_4x8) {
    board::Board board{4, 8};
    auto edge_mask = build_left_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "a2"_n, "a3"_n, "a4"_n, "a5"_n, "a6"_n, "a7"_n, "a8"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskLeft, custom_8x4) {
    board::Board board{8, 4};
    auto edge_mask = build_left_squares_mask(board);
    auto expected = combine_squares(board, "a1"_n, "a2"_n, "a3"_n, "a4"_n);
    EXPECT_EQ(edge_mask, expected);
}

// Right

TEST(BoardEdgeMaskRight, default_8x8) {
    board::Board board{8, 8};
    auto edge_mask = build_right_squares_mask(board);
    auto expected = combine_squares(board, "h1"_n, "h2"_n, "h3"_n, "h4"_n, "h5"_n, "h6"_n, "h7"_n, "h8"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskRight, custom_3x5) {
    board::Board board{3, 5};
    auto edge_mask = build_right_squares_mask(board);
    auto expected = combine_squares(board, "c1"_n, "c2"_n, "c3"_n, "c4"_n, "c5"_n);
    EXPECT_EQ(edge_mask, expected);
}

// Bottom

TEST(BoardEdgeMaskBottom, default_8x8) {
    board::Board board{8, 8};
    auto edge_mask = build_bottom_squares_mask(board);
    auto expected = combine_squares(board, "a8"_n, "b8"_n, "c8"_n, "d8"_n, "e8"_n, "f8"_n, "g8"_n, "h8"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskBottom, custom_4x8) {
    board::Board board{4, 8};
    auto edge_mask = build_bottom_squares_mask(board);
    auto expected = combine_squares(board, "a8"_n, "b8"_n, "c8"_n, "d8"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskBottom, custom_2x8) {
    board::Board board{2, 8};
    auto edge_mask = build_bottom_squares_mask(board);
    auto expected = combine_squares(board, "a8"_n, "b8"_n);
    EXPECT_EQ(edge_mask, expected);
}

TEST(BoardEdgeMaskBottom, custom_9x4) {
    board::Board board{10, 4};
    auto edge_mask = build_bottom_squares_mask(board);
    auto expected = combine_squares(board, "a4"_n, "b4"_n, "c4"_n, "d4"_n, "e4"_n, "f4"_n, "g4"_n, "h4"_n, "i4"_n, "j4"_n);
    EXPECT_EQ(edge_mask, expected);
}