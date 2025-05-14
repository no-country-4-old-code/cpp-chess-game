#include <gtest/gtest.h>
#include "piece-api.h"
#include <board.h>
#include <aggregator-positions.h>
#include "piece-king.h"
#include "piece-rock.h"
#include "notation.h"
#include "piece-api.h"
#include "piece.h"

using namespace piece;
using namespace board::notation::literal;

namespace {
    const board::Board default_board{8, 8};
    board::bitmap::Squares updated_piece_positions = 0;
    unsigned int call_count = 0;

    void mock_update (piece::Piece& piece, const board::Board &board, sqrs pos_all, sqrs pos_hostile_armies) {
        EXPECT_FALSE(updated_piece_positions & piece.position); // function should be called once for each piece
        EXPECT_TRUE(piece.position & pos_all); // piece position should be in "all positions"
        EXPECT_FALSE(piece.position & pos_hostile_armies); // piece position should not be a "hostile position"
        EXPECT_TRUE(pos_hostile_armies > 0); // there should be at least 1 hostile position

        updated_piece_positions |= piece.position;
        ++call_count;
    };

    class PieceMock: public Piece {
        public:
            PieceMock(PieceType type ,board::notation::ChessNotation notation, update_fn mock_update): Piece(type, notation.as_squares(default_board), mock_update) {};
    };
}


TEST(PieceApi_InitArmy, Simple) {
    piece::aggregator::army_list army_list = {
        piece::army::Army{Color::BLUE, {
                                           PieceMock(PieceType::KING, "a1"_n, mock_update),
                                           PieceMock(PieceType::ROCK, "a2"_n, mock_update)
                                       }},
        piece::army::Army{Color::WHITE, {
                                           PieceMock(PieceType::KING, "e4"_n, mock_update)
                                        }},
        piece::army::Army{Color::ORANGE, {
                                           PieceMock(PieceType::KING, "g8"_n, mock_update),
                                           PieceMock(PieceType::ROCK, "g7"_n, mock_update),
                                           PieceMock(PieceType::ROCK, "g6"_n, mock_update)
                                        }}
                                    };
    // act
    piece::api::init_army_list(army_list, default_board);
    // expect
    EXPECT_EQ(call_count, 6);   
}