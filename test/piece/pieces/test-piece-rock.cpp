#include <gtest/gtest.h>
#include "piece-api.h"
#include <board.h>
#include <aggregator-positions.h>
#include "piece-king.h"
#include "piece-rock.h"
#include "notation.h"
#include "piece-api.h"

using namespace piece;
using namespace board::notation::literal;


TEST(Piece_Rock, EmptyField) {
    board::Board board{8, 8};
    auto piece = pieces::Rock{"e4"_n.as_squares(board)};
    
    auto expected_observed = 
        "e1"_n.as_squares(board) | "e2"_n.as_squares(board) | "e3"_n.as_squares(board) | 
        "e5"_n.as_squares(board) | "e6"_n.as_squares(board) | "e7"_n.as_squares(board) | "e8"_n.as_squares(board) |
        "a4"_n.as_squares(board) | "b4"_n.as_squares(board) | "c4"_n.as_squares(board) | "d4"_n.as_squares(board) |
        "f4"_n.as_squares(board) | "g4"_n.as_squares(board) | "h4"_n.as_squares(board);
    auto expected_attackable = expected_observed;
    

    piece.update_observed_and_attackable(board, 0, 0);
    
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_attackable);
}

