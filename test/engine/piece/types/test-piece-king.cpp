#include <board.h>
#include <gtest/gtest.h>
#include "piece-actions.h"

#include <bitset>
#include <iostream>
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

TEST(Piece_King, EmptyField)
{
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  -
2        -  -  -  -  -  -  -  -
3        -  -  -  X  X  X  -  -
4        -  -  -  X  -  X  -  -
5        -  -  -  X  X  X  -  -
6        -  -  -  -  -  -  -  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  -  -  -  -

    */
    Piece piece = King(default_board, "e4"_n);

    auto expected_observed = combine_squares(default_board, "d3"_n, "e3"_n, "f3"_n, "d4"_n, "f4"_n,
                                             "d5"_n, "e5"_n, "f5"_n);
    // act
    piece.update(default_board, 0, 0);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_King, BlockedByFriend)
{
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  -
2        -  -  -  -  -  -  -  -
3        -  -  -  X  X  X  -  -
4        -  -  -  O  -  X  -  -
5        -  -  -  X  X  X  -  -
6        -  -  -  -  -  -  -  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  -  -  -  -

    */
    Piece piece = King(default_board, "e4"_n);
    auto position_friend = "d4"_n.as_squares(default_board);

    auto expected_observed = combine_squares(default_board, "d3"_n, "e3"_n, "f3"_n, "d4"_n, "f4"_n,
                                             "d5"_n, "e5"_n, "f5"_n);
    // act
    piece.update(default_board, position_friend, 0);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    EXPECT_EQ(piece.movable, expected_observed & ~position_friend);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_King, BlockedByEnemy)
{
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  -
2        -  -  -  -  -  -  -  -
3        -  -  -  X  X  X  -  -
4        -  -  -  A  -  X  -  -
5        -  -  -  X  X  X  -  -
6        -  -  -  -  -  -  -  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  -  -  -  -

    */
    Piece piece = King(default_board, "e4"_n);
    auto position_enemy = "d4"_n.as_squares(default_board);

    auto expected_observed = combine_squares(default_board, "d3"_n, "e3"_n, "f3"_n, "d4"_n, "f4"_n,
                                             "d5"_n, "e5"_n, "f5"_n);
    // act
    piece.update(default_board, position_enemy, position_enemy);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_King, BlockedMixed)
{
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  -
2        -  -  -  -  -  -  -  -
3        -  -  -  O  A  X  -  -
4        -  -  -  A  -  X  -  -
5        -  -  -  X  X  O  -  -
6        -  -  -  -  -  -  -  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  A  -  -  -
    */
    Piece piece = King(default_board, "e4"_n);
    auto positions_friend = combine_squares(default_board, "d3"_n, "f5"_n);
    auto positions_enemy = combine_squares(default_board, "d4"_n, "e3"_n, "e8"_n);

    auto expected_observed = combine_squares(default_board, "d3"_n, "e3"_n, "f3"_n, "d4"_n, "f4"_n,
                                             "d5"_n, "e5"_n, "f5"_n);
    // act
    piece.update(default_board, positions_enemy | positions_friend,
                 positions_enemy);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    EXPECT_EQ(piece.movable, expected_observed & ~positions_friend); // should always be the same for king
    // test::helper::display_bits_on_board(default_board, expected_observed);
}
