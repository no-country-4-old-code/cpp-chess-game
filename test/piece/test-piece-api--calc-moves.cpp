#include <gtest/gtest.h>
#include "piece-api.h"
#include <board.h>
#include <aggregator-positions.h>
#include "piece-king.h"
#include "piece-rock.h"
#include "notation.h"
#include "piece-api.h"
#include "piece-update.h"

using namespace piece;
using namespace board::notation::literal;

namespace {

}

TEST(PieceApi_CalcMove, KingMovementObstructed) {
    board::Board board{3, 3};

    piece::aggregator::army_list army_list = {
        piece::army::Army{Color::BLUE, {pieces::King{"b1"_n.as_squares(board)},
                                       }},
        piece::army::Army{Color::WHITE, {
                                            pieces::King{"b3"_n.as_squares(board)},
                                        }}
    };
    piece::api::init_army_list(army_list, board);
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, "a1"_n.as_squares(board) | "c1"_n.as_squares(board));
}

TEST(PieceApi_CalcMove, KingMovementUnderAttack) {
    board::Board board{3, 3};

    piece::aggregator::army_list army_list = {
        piece::army::Army{Color::BLUE, {pieces::King{"b1"_n.as_squares(board)},
                                       }},
        piece::army::Army{Color::WHITE, {
                                            pieces::Rock{"b3"_n.as_squares(board)},
                                        }}
    };
    piece::api::init_army_list(army_list, board);
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, "a1"_n.as_squares(board) | "c1"_n.as_squares(board) | "c2"_n.as_squares(board) | "a2"_n.as_squares(board));
}