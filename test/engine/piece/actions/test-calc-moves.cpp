#include <board.h>
#include <gtest/gtest.h>
#include <iostream>
#include "iterator-bitfield.h"
#include "notation.h"
#include "piece-actions.h"
#include "pieces.h"
#include "test-helper.h"

using namespace piece;
using namespace board::notation::literal;
using namespace test::helper;

namespace {
    void list_squares(const board::bitmap::Squares squares, const board::Board &board) {
        IteratorBitmap iter{squares};
        std::cout << "Squares: ";
        while (*iter) {
            std::cout << board::notation::ChessNotation{*iter, board} << ", ";
            ++iter;
        }
        std::cout << "\n" << std::flush;
    }
}

TEST(PieceApi_CalcMove, KingMovementObstructed) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, {King(board, "b1"_n)}, {King(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);  // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n));
    EXPECT_EQ(moves_all[0].extra.src, 0);
}

TEST(PieceApi_CalcMove, KingMovementUnderAttack) {
    board::Board board{3, 3};
    auto army_list =
        create_army_list(board, {King(board, "b1"_n)}, {King(board, "c3"_n), Rock(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);  // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n, "a2"_n));
    EXPECT_EQ(moves_all[0].extra.src, 0);
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, KingNoMovementPossible) {
    board::Board board{3, 3};
    auto army_list =
        create_army_list(board, {King(board, "a1"_n)},
                         {King(board, "c3"_n), Rock(board, "b3"_n), Rock(board, "c2"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 0);
}

TEST(PieceApi_CalcMove, KingCheckmate) {
    board::Board board{3, 3};
    auto army_list =
        create_army_list(board, {King(board, "a1"_n)},
                         {King(board, "c3"_n), Rock(board, "b3"_n), Rock(board, "a3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 0);
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, KingOnlyMoveIsAttack) {
    board::Board board{2, 3};
    auto army_list = create_army_list(board, {King(board, "a1"_n)}, {King(board, "b2"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);  // only one piece should move
    EXPECT_EQ(moves_all[0].destinations, "b2"_n.as_squares(board));
}

TEST(PieceApi_CalcMove, KingCanNotEscapeAtlongAttackLine) {
    board::Board board{4, 4};
    auto army_list =
        create_army_list(board, {King(board, "b1"_n)}, {King(board, "b3"_n), Rock(board, "d1"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 0);  // no move possibly
}

TEST(PieceApi_CalcMove, TwoPieceOnlyRockUnderAttack) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, {King(board, "a1"_n), Rock(board, "b1"_n)},
                                      {King(board, "c3"_n), Rock(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 2);
    EXPECT_EQ(moves_all[0].src, "a1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a2"_n));
    EXPECT_EQ(moves_all[1].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[1].destinations, combine_squares(board, "c1"_n, "b2"_n, "b3"_n));
    list_squares(moves_all[1].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptImpossible) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, {King(board, "b1"_n), Rock(board, "a1"_n)},
                                      {King(board, "c3"_n), Rock(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a2"_n, "c1"_n));
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptPossible) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, {King(board, "b1"_n), Rock(board, "a2"_n)},
                                      {King(board, "c3"_n), Rock(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 2);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n));
    EXPECT_EQ(moves_all[1].src, "a2"_n.as_squares(board));
    EXPECT_EQ(moves_all[1].destinations,
              combine_squares(board, "b2"_n));  // Piece intercept aggressor
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptPossible2) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, {King(board, "b1"_n), Rock(board, "a3"_n)},
                                      {King(board, "c3"_n), Rock(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 2);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n, "a2"_n));
    EXPECT_EQ(moves_all[1].src, "a3"_n.as_squares(board));
    EXPECT_EQ(moves_all[1].destinations,
              combine_squares(board, "b3"_n));  // Piece eliminates aggressor
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceMovementWouldEndangerKing) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, {King(board, "b1"_n), Rock(board, "b2"_n)},
                                      {King(board, "c3"_n), Rock(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 2);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n, "a2"_n));
    EXPECT_EQ(moves_all[1].src, "b2"_n.as_squares(board));
    EXPECT_EQ(moves_all[1].destinations, combine_squares(board, "b3"_n));
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptWouldEndangerKing) {
    board::Board board{3, 3};
    auto army_list =
        create_army_list(board, {King(board, "c1"_n), Rock(board, "c2"_n)},
                         {Bishop(board, "a3"_n), King(board, "b3"_n), Rock(board, "c3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);
    EXPECT_EQ(moves_all[0].src, "c1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "b1"_n));
    list_squares(moves_all[0].destinations, board);
}


TEST(PieceApi_CalcMove, ShouldSupportCastling) {
    // castling itself is covered by other unittest
    board::Board board{5, 5};
    auto army_list = create_army_list(board, 
                                      {King(board, "a1"_n), Rock(board, "d1"_n)},
                                      {King(board, "a5"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 3);
    EXPECT_EQ(moves_all[2].src, "a1"_n.as_squares(board));
    EXPECT_EQ(moves_all[2].destinations, combine_squares(board, "c1"_n));
    EXPECT_EQ(moves_all[2].extra.src, "d1"_n.as_squares(board));
    EXPECT_EQ(moves_all[2].extra.dest, "b1"_n.as_squares(board));
    list_squares(moves_all[2].destinations, board);
}