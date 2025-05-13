#include <gtest/gtest.h>
#include "piece-api.h"
#include <board.h>
#include <aggregator-positions.h>
#include "piece-king.h"
#include "piece-rock.h"
#include "notation.h"
#include "piece-api.h"
#include <iostream>
#include <bitset>


TEST(Piece_King, EmptyField) {
    using namespace piece;
    using namespace board::notation::literal;

    board::Board board{8, 8};
    auto piece = pieces::King{"e4"_n.as_squares(board)};
    
    auto expected_observed = 
        "e3"_n.as_squares(board) | 
        "d3"_n.as_squares(board) | "d4"_n.as_squares(board) | "d5"_n.as_squares(board) | 
        "e5"_n.as_squares(board) |
        "f3"_n.as_squares(board) | "f4"_n.as_squares(board) | "f5"_n.as_squares(board);
    auto expected_attackable = expected_observed;
    
    std::cout << "Miua " << "KING PIECE\n";
    piece.update_observed_and_attackable(board, 0, 0);
    
    EXPECT_EQ(piece.observed, expected_observed);
    EXPECT_EQ(piece.attackable, expected_attackable);
}