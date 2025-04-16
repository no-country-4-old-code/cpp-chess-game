#include <board-movements.h>
#include <gtest/gtest.h>
#include <notation.h>
#include <squares.h>
#include <sstream>
#include <tuple>
#include "board-movements.h"

using namespace board::notation::literal;

namespace {
    board::Board default_board{8, 8};
}

TEST(BoardMovementsHorizontal, Left) {
    auto start        = "b1"_n;
    auto expected_end = "a1"_n;

    auto end_square =
        board::movements::left(start.as_squares(default_board), default_board);

    board::notation::ChessNotation end{end_square, default_board};
    EXPECT_EQ(end, expected_end);
}
