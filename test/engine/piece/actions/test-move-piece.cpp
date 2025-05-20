#include <gtest/gtest.h>
#include "piece-actions.h"
#include <board.h>

#include "pieces.h"
#include "pieces.h"
#include "notation.h"
#include "test-helper.h"

using namespace piece;
using namespace board::notation::literal;

namespace
{
    const board::Board default_board{8, 8};
    board::bitmap::Squares updated_piece_positions = 0;
    unsigned int call_count = 0;

    void mock_update(piece::Piece &piece, const board::Board &board, sqrs pos_all, sqrs pos_hostile_armies)
    {
        EXPECT_FALSE(updated_piece_positions & piece.position); // function should be called once for each piece
        EXPECT_TRUE(piece.position & pos_all);                  // piece position should be in "all positions"
        EXPECT_FALSE(piece.position & pos_hostile_armies);      // piece position should not be a "hostile position"
        EXPECT_TRUE(pos_hostile_armies > 0);                    // there should be at least 1 hostile position

        updated_piece_positions |= piece.position;
        ++call_count;
    };

    class PieceMock : public Piece
    {
    public:
        PieceMock(PieceType type, board::notation::ChessNotation notation, update_fn mock_update) : Piece(type, notation.as_squares(default_board), mock_update) {};
    };
}

class PieceApi_MovePiece : public ::testing::Test
{
protected:
    piece::army::army_list army_list;

    void SetUp() override
    {
        updated_piece_positions = 0;
        call_count = 0;

        army_list = {
            piece::army::Army{Color::BLUE, {PieceMock(PieceType::KING, "d6"_n, mock_update), PieceMock(PieceType::ROCK, "b1"_n, mock_update)}},
            piece::army::Army{Color::WHITE, {PieceMock(PieceType::KING, "g7"_n, mock_update), PieceMock(PieceType::ROCK, "e8"_n, mock_update)}},
            piece::army::Army{Color::ORANGE, {PieceMock(PieceType::KING, "g1"_n, mock_update), PieceMock(PieceType::ROCK, "d8"_n, mock_update)}}};
        /*
             a   b   c   d   e   f   g   h
        1            Rb                  Ko
        2
        3
        4
        5
        6                    Kb
        7                                Kw
        8                    Ro  Rw
        */
    }
};

TEST_F(PieceApi_MovePiece, MoveUnseen)
{
    auto &piece = army_list[0].pieces[1];
    auto src = piece.position;
    auto dest = "b7"_n.as_squares(default_board);
    auto backup = army_list;
    // act
    piece::api::move_piece(src, dest, default_board, army_list);
    // expect
    EXPECT_EQ(call_count, 1); // should only bother the moved piece
    EXPECT_EQ(updated_piece_positions, dest);
    EXPECT_EQ(piece.position, dest);
    EXPECT_TRUE(army_list[0].pieces[0] == backup[0].pieces[0]);
    EXPECT_TRUE(army_list[1].pieces[0] == backup[1].pieces[0]);
    EXPECT_TRUE(army_list[1].pieces[1] == backup[1].pieces[1]);
    EXPECT_TRUE(army_list[2].pieces[0] == backup[2].pieces[0]);
    EXPECT_TRUE(army_list[2].pieces[1] == backup[2].pieces[1]);
}

TEST_F(PieceApi_MovePiece, MoveUnseenWithAttack)
{
    auto &piece = army_list[0].pieces[1];
    auto src = piece.position;
    auto dest = "g1"_n.as_squares(default_board);
    auto backup = army_list;
    // act
    piece::api::move_piece(src, dest, default_board, army_list);
    // expect
    EXPECT_EQ(call_count, 1); // should only bother the moved piece
    EXPECT_EQ(updated_piece_positions, dest);
    EXPECT_EQ(piece.position, dest);
    EXPECT_FALSE(army_list[2].pieces[0].is_alive()); // <-- should be dead
    EXPECT_TRUE(army_list[0].pieces[0] == backup[0].pieces[0]);
    EXPECT_TRUE(army_list[1].pieces[0] == backup[1].pieces[0]);
    EXPECT_TRUE(army_list[1].pieces[1] == backup[1].pieces[1]);
    EXPECT_TRUE(army_list[2].pieces[1] == backup[2].pieces[1]);
}

TEST_F(PieceApi_MovePiece, MoveFromObservedToUnobserved)
{
    auto &piece = army_list[0].pieces[0];
    auto src = piece.position;
    auto dest = "c6"_n.as_squares(default_board);
    auto &observing_piece = army_list[2].pieces[1];
    observing_piece.observed = src; // set observed manually here
    auto backup = army_list;
    // act
    piece::api::move_piece(src, dest, default_board, army_list);
    // expect
    EXPECT_EQ(call_count, 2);
    EXPECT_EQ(updated_piece_positions, dest | observing_piece.position);
    EXPECT_EQ(piece.position, dest);

    EXPECT_TRUE(army_list[0].pieces[1] == backup[0].pieces[1]);
    EXPECT_TRUE(army_list[1].pieces[0] == backup[1].pieces[0]);
    EXPECT_TRUE(army_list[1].pieces[1] == backup[1].pieces[1]);
    EXPECT_TRUE(army_list[2].pieces[0] == backup[2].pieces[0]);
}

TEST_F(PieceApi_MovePiece, MovedMixed)
{
    auto &piece = army_list[2].pieces[1]; // e8
    auto src = piece.position;
    auto dest = "e1"_n.as_squares(default_board);
    army_list[1].pieces[1].observed = src;
    army_list[0].pieces[1].observed = dest;
    army_list[1].pieces[0].observed = dest | src;
    auto backup = army_list;
    // act
    piece::api::move_piece(src, dest, default_board, army_list);
    // expect
    EXPECT_EQ(call_count, 4);
    EXPECT_EQ(piece.position, dest);
}