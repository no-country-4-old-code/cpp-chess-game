#include <board.h>
#include <gtest/gtest.h>
#include <iostream>
#include "iterator-bitfield.h"
#include "notation.h"
#include "piece-actions.h"
#include "pieces.h"
#include "test-helper.h"
#include "color.h"

using namespace piece;
using namespace board::notation::literal;
using namespace test::helper;

namespace {

    api::Context map_army_to_context(const piece::army::Army &my_army, const piece::army::army_list& army_list) {
        board::bitmap::Squares positions_all_armies = 0;
        board::bitmap::Squares enemy_attack_map = 0;

        for (const auto &army : army_list)
        {
            for (const auto &piece : army.pieces)
            {
                positions_all_armies |= piece.position;
                if (army.color() != my_army.color())
                    {
                        enemy_attack_map |= piece.attackable;
                    }
            }
        }
        return {
            .positions_all_armies = positions_all_armies,
            .under_attack_map = enemy_attack_map
        };
    }


    api::ArmyDestinations act(const piece::army::Army &my_army, const piece::army::army_list& army_list, const board::Board &board) {
        api::Context context = map_army_to_context(my_army, army_list);
        api::ArmyDestinations memory{};
        utils::calc_special_moves(my_army, board, context, memory);
        return memory;
    }

}

TEST(PieceAction_Castling, SimpleCastleMoveLeft) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n)});
    // act
    auto castling_moves = act(army_list[0], army_list, board);
    // assert
    EXPECT_EQ(castling_moves.size(), 1);
    EXPECT_EQ(castling_moves[0].src, "a1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].destinations, "c1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.src, "d1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.dest, "b1"_n.as_squares(board));
}


TEST(PieceAction_Castling, CastleMoveIfMoreThen2SquaresBetween) {
    board::Board board{20, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "b1"_n), Rock(board, "n1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 1);
    EXPECT_EQ(castling_moves[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].destinations, "d1"_n.as_squares(board));
}

TEST(PieceAction_Castling, SimpleCastleMoveRight) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "d1"_n), Rock(board, "a1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 1);
    EXPECT_EQ(castling_moves[0].src, "d1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].destinations, "b1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.src, "a1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.dest, "c1"_n.as_squares(board));
}

TEST(PieceAction_Castling, SimpleCastleMoveDown) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "d1"_n), Rock(board, "d5"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 1);
    EXPECT_EQ(castling_moves[0].src, "d1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].destinations, "d3"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.src, "d5"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.dest, "d2"_n.as_squares(board));
}

TEST(PieceAction_Castling, SimpleCastleMoveUp) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "d5"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 1);
    EXPECT_EQ(castling_moves[0].src, "d5"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].destinations, "d3"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.src, "d1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].extra.dest, "d4"_n.as_squares(board));
}

// === Multiple Castle-Moves possible

TEST(PieceAction_Castling, MultipleCastleMovesPossible) {
    board::Board board{8, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "b1"_n), Rock(board, "h1"_n), Rock(board, "b5"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 2);
    EXPECT_EQ(castling_moves[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[0].destinations, "d1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[1].src, "b1"_n.as_squares(board));
    EXPECT_EQ(castling_moves[1].destinations, "b3"_n.as_squares(board));
}


// === Negative


TEST(PieceAction_Castling, NoLeftSideCastleMoveIfNoSquareBetween) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "b1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoLeftSideCastleMoveIfOneSquareBetween) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "c1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoRightSideCastleMoveIfNoSquareBetween) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "b1"_n), Rock(board, "a1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoRightSideCastleMoveIfOneSquareBetween) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "c1"_n), Rock(board, "a1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoCastleMoveIfKingHasMoved) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n)});
    army_list[0].pieces[0].move("a1"_n.as_squares(board));
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
    EXPECT_TRUE(army_list[0].king().has_moved());
}

TEST(PieceAction_Castling, NoCastleMoveIfRockHasMoved) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n)});
    army_list[0].pieces[1].move("d1"_n.as_squares(board));
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
    EXPECT_TRUE(army_list[0].pieces[1].has_moved());
}

TEST(PieceAction_Castling, NoCastleMoveIfPieceBetween) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Queen(board, "c1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoCastleMoveIfPieceBetweenEvenIfItIsRock) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "c1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}


TEST(PieceAction_Castling, NoCastleMoveIfKingIsUnderAttackAtCurrentSqr) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n), Rock(board, "a4"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoCastleMoveIfKingIsUnderAttackAtTransitSqr) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n), Rock(board, "b4"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoCastleMoveIfKingIsUnderAttackAtDestSqr) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n), Rock(board, "c4"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}

TEST(PieceAction_Castling, NoCastleMoveIfRockIsNotInStraightLineWithKing) {
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "b2"_n), Rock(board, "c3"_n)},
                                      {King(board, "a5"_n)});
    auto castling_moves = act(army_list[0], army_list, board);
    EXPECT_EQ(castling_moves.size(), 0);
}
