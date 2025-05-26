#include <board.h>
#include <gtest/gtest.h>
#include "notation.h"
#include "piece-actions.h"
#include "pieces.h"
#include "test-helper.h"

using namespace piece;
using namespace board::notation::literal;

namespace {
    const board::Board default_board{8, 8};
}

TEST(Piece_Bishop, EmptyField) {
    /*
         a  b  c  d  e  f  g  h
1        -  X  -  -  -  -  -  X
2        -  -  X  -  -  -  X  -
3        -  -  -  X  -  X  -  -
4        -  -  -  -  -  -  -  -
5        -  -  -  X  -  X  -  -
6        -  -  X  -  -  -  X  -
7        -  X  -  -  -  -  -  X
8        X  -  -  -  -  -  -  -

    */
    Piece piece = Bishop(default_board, "e4"_n);

    auto expected_observed =
        combine_squares(default_board, 
                        "b1"_n, "c2"_n, "d3"_n, "f5"_n, "g6"_n, "h7"_n, "a8"_n,
                        "b7"_n, "c6"_n, "d5"_n, "f3"_n, "g2"_n, "h1"_n
                    );
    // act
    piece.update_observed_and_attackable(default_board, 0, 0);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_Bishop, BlockedByFriend) {
    /*
         a  b  c  d  e  f  g  h
1        -  X  -  -  -  -  -  X
2        -  -  X  -  -  -  X  -
3        -  -  -  X  -  X  -  -
4        -  -  -  -  -  -  -  -
5        -  -  -  X  -  X  -  -
6        -  -  O  -  -  -  X  -
7        -  -  -  -  -  -  -  X
8        -  -  -  -  -  -  -  -

    */
    Piece piece          = Bishop(default_board, "e4"_n);
    auto position_friend = "c6"_n.as_squares(default_board);

    auto expected_observed = combine_squares(default_board, "b1"_n, "c2"_n, "d3"_n, "f5"_n, "g6"_n,
                                             "h7"_n, "c6"_n, "d5"_n, "f3"_n, "g2"_n, "h1"_n);
    auto expected_attackable = expected_observed & ~position_friend;
    // act
    piece.update_observed_and_attackable(default_board, position_friend, 0);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_attackable);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_Bishop, BlockedByEnemy) {
    /*
         a  b  c  d  e  f  g  h
1        -  X  -  -  -  -  -  X
2        -  -  X  -  -  -  X  -
3        -  -  -  X  -  X  -  -
4        -  -  -  -  -  -  -  -
5        -  -  -  X  -  X  -  -
6        -  -  A  -  -  -  X  -
7        -  -  -  -  -  -  -  X
8        -  -  -  -  -  -  -  -

    */
    Piece piece         = Bishop(default_board, "e4"_n);
    auto position_enemy = "c6"_n.as_squares(default_board);

    auto expected_observed = combine_squares(default_board, "b1"_n, "c2"_n, "d3"_n, "f5"_n, "g6"_n,
                                             "h7"_n, "c6"_n, "d5"_n, "f3"_n, "g2"_n, "h1"_n);
    // act
    piece.update_observed_and_attackable(default_board, position_enemy, position_enemy);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_observed);
    // test::helper::display_bits_on_board(default_board, expected_observed);
}

TEST(Piece_Bishop, BlockedMixed) {
    /*
         a  b  c  d  e  f  g  h
1        -  -  -  -  -  -  -  X
2        -  -  A  -  A  -  X  -
3        -  -  -  X  -  X  -  -
4        -  -  -  -  -  -  -  -
5        -  -  -  X  -  X  -  -
6        -  -  O  -  -  -  O  -
7        -  -  -  -  -  -  -  -
8        -  -  -  -  A  -  -  -
    */
    Piece piece           = Bishop(default_board, "e4"_n);
    auto positions_friend = combine_squares(default_board, "c6"_n, "g6"_n);
    auto positions_enemy  = combine_squares(default_board, "c2"_n, "e2"_n, "e8"_n);

    auto expected_observed = combine_squares(default_board, "c2"_n, "d3"_n, "f5"_n, "g6"_n, "c6"_n,
                                             "d5"_n, "f3"_n, "g2"_n, "h1"_n);
    auto expected_attackable = expected_observed & ~positions_friend;
    // act
    piece.update_observed_and_attackable(default_board, positions_enemy | positions_friend,
                                         positions_enemy);
    // expect
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_attackable);
    test::helper::display_bits_on_board(default_board, expected_observed);
}
