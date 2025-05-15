#include <gtest/gtest.h>
#include "piece-api.h"
#include <board.h>
#include <iostream>
#include "notation.h"
#include "iterator-bitfield.h"
#include "piece-embraced-squares-mask.h"

using namespace piece;
using namespace board::notation::literal;
using sqrs = board::bitmap::Squares;

template <typename... Squares>
board::bitmap::Squares combine_squares(board::Board board, Squares... squares) {
    return (squares.as_squares(board) | ...);
}

namespace {
    void list_squares(const board::bitmap::Squares squares,const board::Board& board) {
        IteratorBitmap iter{squares};
        std::cout << "Squares: ";
        while (*iter) {
            std::cout << board::notation::ChessNotation{*iter, board} << ", ";
            ++iter;
        }
        std::cout << "\n" << std::flush;
    }

    sqrs act(board::notation::ChessNotation pos1, board::notation::ChessNotation pos2, board::Board board) {
        return utils::create_embraced_squares_mask(pos1.as_squares(board), pos2.as_squares(board), board);
    }
}

TEST(PieceEmbracedSqrsMask, Simple) {
    board::Board board{3, 3};
    auto result = act("a1"_n, "c3"_n, board);
    EXPECT_EQ(result, "b2"_n.as_squares(board));
    list_squares(result, board);
}
