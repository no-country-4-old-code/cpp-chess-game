#include <board.h>
#include <gtest/gtest.h>
#include "piece-actions.h"

#include "notation.h"
#include "piece-actions.h"
#include "pieces.h"
#include "test-helper.h"

using namespace piece;
using namespace board::notation::literal;

namespace
{
    const board::Board default_board{8, 8};
}

TEST(Piece_Knight, EmptyField)
{
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  -
2        -  -  -  X  -  X  -  -
3        -  -  X  -  -  -  X  -
4        -  -  -  -  -  -  -  -
5        -  -  X  -  -  -  X  -
6        -  -  -  X  -  X  -  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  -  -  -  -

    */
    Piece piece = Knight(default_board, "e4"_n);

    auto expected_observed =
        combine_squares(default_board, "d2"_n, "f2"_n, "c3"_n, "g3"_n, "c5"_n, "g5"_n, "d6"_n, "f6"_n);
    // act
    piece.update(default_board, 0, 0);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    EXPECT_EQ(piece.movable, piece.attackable); // should always be the same for knight
    // test::helper::display_bits_on_board(default_board, piece.observed);
}

TEST(Piece_Knight, BlockedMixed)
{
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  -
2        -  -  -  O  -  X  -  -
3        -  -  X  -  O  -  X  -
4        -  -  -  -  -  -  -  -
5        -  -  X  -  -  A  A  -
6        -  -  -  A  -  X  -  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  -  -  -  -
    */
    Piece piece = Knight(default_board, "e4"_n);
    auto positions_friend = combine_squares(default_board, "d2"_n, "e3"_n);
    auto positions_enemy = combine_squares(default_board, "g5"_n, "d6"_n, "f5"_n);

    auto expected_observed = combine_squares(default_board, "d2"_n, "f2"_n, "c3"_n, "g3"_n, "c5"_n, "g5"_n, "d6"_n, "f6"_n);
    // act
    piece.update(default_board, positions_enemy | positions_friend,
                 positions_enemy);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    EXPECT_EQ(piece.movable, expected_observed & ~positions_friend); // should always be the same for knight
    // test::helper::display_bits_on_board(default_board, expected_observed);
}
