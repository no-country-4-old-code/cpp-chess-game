#include <board.h>
#include <gtest/gtest.h>
#include "piece-actions.h"

#include "notation.h"
#include "piece-actions.h"
#include "pieces.h"
#include "test-helper.h"

using namespace piece;
using namespace board::notation::literal;

namespace {
    const board::Board default_board{8, 8};
}

TEST(Piece_Rock, EmptyField) {
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  X  -  -  -
2        -  -  -  -  X  -  -  -
3        -  -  -  -  X  -  -  -
4        X  X  X  X  -  X  X  X
5        -  -  -  -  X  -  -  -
6        -  -  -  -  X  -  -  -
7        -  -  -  -  X  -  -  -
8        -  -  -  -  X  -  -  -

    */
    Piece piece = Rock(default_board, "e4"_n);

    auto expected_observed =
        combine_squares(default_board, "e1"_n, "e2"_n, "e3"_n, "e5"_n, "e6"_n, "e7"_n, "e8"_n,
                        "a4"_n, "b4"_n, "c4"_n, "d4"_n, "f4"_n, "g4"_n, "h4"_n);
    // act
    piece.update_observed_and_attackable(default_board, 0, 0);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_Rock, BlockedByFriend) {
    /*
        a  b  c  d  e  f  g  h
1        -  -  -  -  X  -  -  -
2        -  -  -  -  X  -  -  -
3        -  -  -  -  X  -  -  -
4        X  X  X  X  -  X  O  -
5        -  -  -  -  X  -  -  -
6        -  -  -  -  X  -  -  -
7        -  -  -  -  X  -  -  -
8        -  -  -  -  X  -  -  -

    */
    Piece piece          = Rock(default_board, "e4"_n);
    auto position_friend = "g4"_n.as_squares(default_board);

    auto expected_observed =
        combine_squares(default_board, "e1"_n, "e2"_n, "e3"_n, "e5"_n, "e6"_n, "e7"_n, "e8"_n,
                        "a4"_n, "b4"_n, "c4"_n, "d4"_n, "f4"_n, "g4"_n);
    auto expected_attackable = expected_observed & ~position_friend;
    // act
    piece.update_observed_and_attackable(default_board, position_friend, 0);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_attackable);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_Rock, BlockedByEnemy) {
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  X  -  -  -
2        -  -  -  -  X  -  -  -
3        -  -  -  -  X  -  -  -
4        X  X  X  X  -  X  A  -
5        -  -  -  -  X  -  -  -
6        -  -  -  -  X  -  -  -
7        -  -  -  -  X  -  -  -
8        -  -  -  -  X  -  -  -

    */
    Piece piece         = Rock(default_board, "e4"_n);
    auto position_enemy = "g4"_n.as_squares(default_board);

    auto expected_observed =
        combine_squares(default_board, "e1"_n, "e2"_n, "e3"_n, "e5"_n, "e6"_n, "e7"_n, "e8"_n,
                        "a4"_n, "b4"_n, "c4"_n, "d4"_n, "f4"_n, "g4"_n);
    // act
    piece.update_observed_and_attackable(default_board, position_enemy, position_enemy);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_Rock, BlockedMixed) {
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  -
2        -  -  -  -  O  -  -  -
3        -  -  -  -  X  -  -  -
4        O  X  X  X  -  X  A  -
5        -  -  -  -  X  -  -  -
6        -  -  -  -  A  -  -  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  O  -  -  -
    */
    Piece piece           = Rock(default_board, "e4"_n);
    auto positions_friend = combine_squares(default_board, "g4"_n, "e6"_n);
    auto positions_enemy  = combine_squares(default_board, "a4"_n, "e2"_n, "e8"_n);

    auto expected_observed = combine_squares(default_board, "e2"_n, "e3"_n, "e5"_n, "e6"_n, "a4"_n,
                                             "b4"_n, "c4"_n, "d4"_n, "f4"_n, "g4"_n);
    auto expected_attackable = expected_observed & ~positions_friend;
    // act
    piece.update_observed_and_attackable(default_board, positions_enemy | positions_friend,
                                         positions_enemy);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_attackable);
    EXPECT_EQ(piece.movable, piece.attackable); // should always be the same for rock
    // test::helper::display_bits_on_board(default_board, expected_observed);
}
