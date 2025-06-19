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

    struct Expectation {
        board::bitmap::Squares observed;
        board::bitmap::Squares attackable;
        board::bitmap::Squares movable;
    };

    struct Context {
        board::bitmap::Squares positions_friendly;
        board::bitmap::Squares positions_hostile;
    };

    void act(piece::Piece& piece, Context& context, Expectation& expectation) {
        auto positions_all = context.positions_friendly | context.positions_hostile;
        piece.update(default_board, positions_all, context.positions_hostile);
        // expect
        EXPECT_EQ(piece.observed, expectation.observed);
        EXPECT_EQ(piece.attackable, expectation.attackable);
        EXPECT_EQ(piece.movable, expectation.movable);
        test::helper::display_bits_on_board(default_board, piece.observed);
        test::helper::display_bits_on_board(default_board, piece.movable);
    }

}


TEST(Piece_Pawn, ShouldBeAbleToMove2StepsAtFirstMove)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n, "e6"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n, "e6"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeNotAbleToMove1StepIfBlockedByFriend)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = combine_squares(default_board, "e4"_n, "e5"_n),
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = 0
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeNotAbleToMove2StepsIfBlockedByFriend)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = combine_squares(default_board, "e4"_n, "e6"_n),
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n, "e6"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeNotAbleToMove2StepsIfBlockedByEnemy)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = combine_squares(default_board, "e4"_n, "e6"_n),
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n, "e6"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeNotAbleToMove1StepIfBlockedByEnemy)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = combine_squares(default_board, "e4"_n, "e5"_n),
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = 0
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldNotBeAbleToMove2StepsAfterMoved)
{
    Piece piece = Pawn(default_board, "e3"_n, MoveDirection::DOWN);
    piece.move("e4"_n.as_squares(default_board));

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeAbleToMoveSidewaysOnAttackLeft)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = combine_squares(default_board, "d5"_n),
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n, "e6"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n, "e6"_n, "d5"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeAbleToMoveSidewaysOnAttackRight)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = combine_squares(default_board, "f5"_n),
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n, "e6"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n, "e6"_n, "f5"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeAbleToMoveSidewaysOnAttackBoth)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::DOWN);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = combine_squares(default_board, "d5"_n, "f5"_n),
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n, "e6"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n, "e6"_n, "d5"_n, "f5"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeAbleToMoveSidewaysOnAttackBothAfterMove)
{
    Piece piece = Pawn(default_board, "e3"_n, MoveDirection::DOWN);
    piece.move("e4"_n.as_squares(default_board));

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = combine_squares(default_board, "d5"_n, "f5"_n),
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "e5"_n, "f5"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "e5"_n, "d5"_n, "f5"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn, ShouldBeAbleToMoveSidewaysOnAttackBothAfterMoveAndBlocked)
{
    Piece piece = Pawn(default_board, "e3"_n, MoveDirection::DOWN);
    piece.move("e4"_n.as_squares(default_board));

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = combine_squares(default_board, "e5"_n, "d5"_n, "f5"_n),
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d5"_n, "f5"_n, "e5"_n),
        .attackable = combine_squares(default_board, "d5"_n, "f5"_n),
        .movable = combine_squares(default_board, "d5"_n, "f5"_n)
    };

    act(piece, context, expect);
}

// === Check Pawns with other directions

TEST(Piece_Pawn_UP, ShouldBeAbleToMove2StepsAtFirstMove)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::UP);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d3"_n, "e3"_n, "f3"_n, "e2"_n),
        .attackable = combine_squares(default_board, "d3"_n, "f3"_n),
        .movable = combine_squares(default_board, "e3"_n, "e2"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn_LEFT, ShouldBeAbleToMove2StepsAtFirstMove)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::LEFT);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d3"_n, "d4"_n, "d5"_n, "c4"_n),
        .attackable = combine_squares(default_board, "d3"_n, "d5"_n),
        .movable = combine_squares(default_board, "d4"_n, "c4"_n)
    };

    act(piece, context, expect);
}

TEST(Piece_Pawn_RIGHT, ShouldBeAbleToMove2StepsAtFirstMove)
{
    Piece piece = Pawn(default_board, "e4"_n, MoveDirection::RIGHT);

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "f3"_n, "f4"_n, "f5"_n, "g4"_n),
        .attackable = combine_squares(default_board, "f3"_n, "f5"_n),
        .movable = combine_squares(default_board, "f4"_n, "g4"_n)
    };

    act(piece, context, expect);
}

// === Test envolving

TEST(Piece_Pawn_Envolvment, ShouldEnvolveInStrongerPieceAtEndOfBoard)
{
    Piece piece = Pawn(default_board, "e7"_n, MoveDirection::DOWN);
    piece.move("e8"_n.as_squares(default_board));

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = 0
    };

    auto queen_map = combine_squares(default_board, "a8"_n, "b8"_n, "c8"_n, "d8"_n, "f8"_n, "g8"_n, "h8"_n, 
                                                   "e1"_n, "e2"_n, "e3"_n, "e4"_n, "e5"_n, "e6"_n, "e7"_n,
                                                   "a4"_n, "b5"_n, "c6"_n, "d7"_n, 
                                                   "f7"_n, "g6"_n, "h5"_n);
    Expectation expect = {
        .observed = queen_map,
        .attackable = queen_map,
        .movable = queen_map 
    };

    act(piece, context, expect);
    EXPECT_EQ(piece.type, piece::PieceType::QUEEN);
}

TEST(Piece_Pawn, ShouldNotEnvolveDirectionNotMatchEdge)
{
    Piece piece = Pawn(default_board, "e7"_n, MoveDirection::LEFT);
    piece.move("e8"_n.as_squares(default_board));

    Context context = {
        .positions_friendly = piece.position,
        .positions_hostile  = 0
    };

    Expectation expect = {
        .observed = combine_squares(default_board, "d8"_n, "d7"_n),
        .attackable = combine_squares(default_board, "d7"_n),
        .movable = combine_squares(default_board, "d8"_n)
    };

    act(piece, context, expect);
    EXPECT_EQ(piece.type, piece::PieceType::PAWN);
}

TEST(Piece_Pawn_LEFT, ShouldEnvolveInStrongerPieceAtEndOfBoard)
{
    Piece piece = Pawn(default_board, "a6"_n, MoveDirection::LEFT);
    piece.update(default_board, piece.position, 0);
    EXPECT_EQ(piece.type, piece::PieceType::QUEEN);
}

TEST(Piece_Pawn_LEFT, ShouldNotEnvolveInStrongerPieceAtWrongEdge)
{
    Piece piece = Pawn(default_board, "b1"_n, MoveDirection::LEFT);
    piece.update(default_board, piece.position, 0);
    EXPECT_EQ(piece.type, piece::PieceType::PAWN);
}

TEST(Piece_Pawn_UP, ShouldEnvolveInStrongerPieceAtEndOfBoard)
{
    Piece piece = Pawn(default_board, "b1"_n, MoveDirection::UP);
    piece.update(default_board, piece.position, 0);
    EXPECT_EQ(piece.type, piece::PieceType::QUEEN);
}

TEST(Piece_Pawn_UP, ShouldNotEnvolveInStrongerPieceAtWrongEdge)
{
    Piece piece = Pawn(default_board, "h4"_n, MoveDirection::UP);
    piece.update(default_board, piece.position, 0);
    EXPECT_EQ(piece.type, piece::PieceType::PAWN);
}

TEST(Piece_Pawn_RIGHT, ShouldEnvolveInStrongerPieceAtEndOfBoard)
{
    Piece piece = Pawn(default_board, "h4"_n, MoveDirection::RIGHT);
    piece.update(default_board, piece.position, 0);
    EXPECT_EQ(piece.type, piece::PieceType::QUEEN);
}

TEST(Piece_Pawn_RIGHT, ShouldNotEnvolveInStrongerPieceAtWrongEdge)
{
    Piece piece = Pawn(default_board, "d8"_n, MoveDirection::RIGHT);
    piece.update(default_board, piece.position, 0);
    EXPECT_EQ(piece.type, piece::PieceType::PAWN);
}