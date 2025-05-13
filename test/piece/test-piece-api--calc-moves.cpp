#include <gtest/gtest.h>
#include "piece-api.h"
#include <board.h>
#include <aggregator-positions.h>
#include "piece-king.h"
#include "piece-rock.h"
#include "notation.h"
#include "piece-api.h"

TEST(PieceApi_CalcMove, Dummy) {
    board::Board board{8, 8};

    using namespace piece;
    using namespace board::notation::literal;

    piece::aggregator::army_list army_list = {
        piece::army::Army{Color::BLUE, {
                                           pieces::King{"a1"_n.as_squares(board)},
                                           pieces::Rock{"e4"_n.as_squares(board)},
                                           pieces::Rock{"b4"_n.as_squares(board)},
                                           pieces::Rock{"b6"_n.as_squares(board)},
                                       }},
        piece::army::Army{Color::WHITE, {
                                            pieces::King{"h1"_n.as_squares(board)},
                                            pieces::Rock{"f8"_n.as_squares(board)},
                                            pieces::Rock{"g7"_n.as_squares(board)},
                                            pieces::Rock{"h6"_n.as_squares(board)},
                                        }},
        piece::army::Army{Color::ORANGE, {pieces::King{"d8"_n.as_squares(board)}, pieces::Rock{"d7"_n.as_squares(board)}}}, piece::army::Army{}};

    piece::api::init_army_list(army_list, board);
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
}