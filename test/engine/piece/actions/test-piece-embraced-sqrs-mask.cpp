#include <board.h>
#include <gtest/gtest.h>
#include <iostream>
#include "iterator-bitfield.h"
#include "notation.h"
#include "piece-actions.h"
#include "test-helper.h"

using namespace piece;
using namespace board::notation::literal;
using sqrs = board::bitmap::Squares;

namespace {
    void list_squares(const board::bitmap::Squares squares,
                      const board::Board &board) {
        IteratorBitmap iter{squares};
        std::cout << "Squares: ";
        while (*iter) {
            std::cout << board::notation::ChessNotation{*iter, board} << ", ";
            ++iter;
        }
        std::cout << "\n" << std::flush;
    }

    sqrs act(board::notation::ChessNotation pos1,
             board::notation::ChessNotation pos2, board::Board board) {
        return utils::create_embraced_squares_mask(
            pos1.as_squares(board), pos2.as_squares(board), board);
    }
}

TEST(PieceEmbracedSqrsMask, DiagonalLeft) {
    board::Board board{3, 3};
    auto result = act("a1"_n, "c3"_n, board);
    EXPECT_EQ(result, "b2"_n.as_squares(board));
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, DiagonalRight) {
    board::Board board{3, 3};
    auto result = act("c1"_n, "a3"_n, board);
    EXPECT_EQ(result, "b2"_n.as_squares(board));
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, StraightUpDown) {
    board::Board board{3, 3};
    auto result = act("b1"_n, "b3"_n, board);
    EXPECT_EQ(result, "b2"_n.as_squares(board));
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, StraightLeftRight) {
    board::Board board{3, 3};
    auto result = act("a2"_n, "c2"_n, board);
    EXPECT_EQ(result, "b2"_n.as_squares(board));
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, NoInbetweenDiagonal) {
    board::Board board{3, 3};
    auto result = act("a1"_n, "b2"_n, board);
    EXPECT_EQ(result, 0);
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, NoInbetweenSideways) {
    board::Board board{3, 3};
    auto result = act("a2"_n, "b2"_n, board);
    EXPECT_EQ(result, 0);
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, NoInbetweenUpDown) {
    board::Board board{3, 3};
    auto result = act("b2"_n, "b3"_n, board);
    EXPECT_EQ(result, 0);
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, NoInbetweenDiagonalBigger1) {
    board::Board board{3, 3};
    auto result = act("c3"_n, "b1"_n, board);
    EXPECT_EQ(result, 0);
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, NoInbetweenDiagonalBigger2) {
    board::Board board{3, 3};
    auto result = act("a1"_n, "b3"_n, board);
    EXPECT_EQ(result, 0);
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, TwoInbetweenUpDown) {
    board::Board board{8, 8};
    auto result = act("b2"_n, "b5"_n, board);
    EXPECT_EQ(result, combine_squares(board, "b3"_n, "b4"_n));
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, TwoInbetweenSideways) {
    board::Board board{8, 8};
    auto result = act("b4"_n, "e4"_n, board);
    EXPECT_EQ(result, combine_squares(board, "c4"_n, "d4"_n));
    list_squares(result, board);
}

TEST(PieceEmbracedSqrsMask, TwoInbetweenDiagonal1) {
    board::Board board{8, 8};
    auto result = act("c4"_n, "f7"_n, board);
    EXPECT_EQ(result, combine_squares(board, "d5"_n, "e6"_n));
    list_squares(result, board);
}
