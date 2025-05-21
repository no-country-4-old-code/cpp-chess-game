#include <gtest/gtest.h>
#include "piece-actions.h"
#include <board.h>
#include "pieces.h"
#include "pieces.h"
#include "pieces.h"
#include "notation.h"
#include "piece-actions.h"
#include "test-helper.h"
#include "iterator-bitfield.h"
#include "test-helper.h"
#include <iostream>

using namespace piece;
using namespace board::notation::literal;

namespace
{
    void list_squares(const board::bitmap::Squares squares, const board::Board &board)
    {
        IteratorBitmap iter{squares};
        std::cout << "Squares: ";
        while (*iter)
        {
            std::cout << board::notation::ChessNotation{*iter, board} << ", ";
            ++iter;
        }
        std::cout << "\n"
                  << std::flush;
    }

    piece::army::army_list create_army_list(board::Board board, std::vector<piece::Piece> white, std::vector<piece::Piece> black)
    {
        piece::army::army_list army_list = {
            piece::army::Army{Color::WHITE, white},
            piece::army::Army{Color::BLACK, black}};
        piece::api::init_army_list(army_list, board);
        return army_list;
    }
}

TEST(PieceApi_CalcMove, KingMovementObstructed)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "b1"_n)},
                                      {King(board, "b3"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n));
}

TEST(PieceApi_CalcMove, KingMovementUnderAttack)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "b1"_n)},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n, "a2"_n));
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, KingNoMovementPossible)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "a1"_n)},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}, Rock{board, "c2"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 0);
}

TEST(PieceApi_CalcMove, KingCheckmate)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "a1"_n)},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}, Rock{board, "a3"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 0);
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, KingOnlyMoveIsAttack)
{
    board::Board board{2, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "a1"_n)},
                                      {King(board, "b2"_n)});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].destinations, "b2"_n.as_squares(board));
}

TEST(PieceApi_CalcMove, TwoPieceOnlyRockUnderAttack)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "a1"_n), Rock{board, "b1"_n}},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}});
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

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptImpossible)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "b1"_n), Rock{board, "a1"_n}},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a2"_n, "c1"_n));
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptPossible)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "b1"_n), Rock{board, "a2"_n}},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 2);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n));
    EXPECT_EQ(moves_all[1].src, "a2"_n.as_squares(board));
    EXPECT_EQ(moves_all[1].destinations, combine_squares(board, "b2"_n)); // rock intercept aggressor
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptPossible2)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "b1"_n), Rock{board, "a3"_n}},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 2);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n, "a2"_n));
    EXPECT_EQ(moves_all[1].src, "a3"_n.as_squares(board));
    EXPECT_EQ(moves_all[1].destinations, combine_squares(board, "b3"_n)); // rock eliminates aggressor
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceMovementWouldEndangerKing)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "b1"_n), Rock{board, "b2"_n}},
                                      {King(board, "c3"_n), Rock{board, "b3"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "a1"_n, "c1"_n, "a2"_n));
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, TwoPieceKingUnderAttackOneAttackerInterceptWouldEndangerKing)
{
    board::Board board{3, 3};
    auto army_list = create_army_list(board,
                                      {King(board, "c1"_n), Rock{board, "c2"_n}},
                                      {Bishop(board, "a3"_n), King(board, "b3"_n), Rock{board, "c3"_n}});
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1);
    EXPECT_EQ(moves_all[0].src, "c1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, combine_squares(board, "b1"_n));
    list_squares(moves_all[0].destinations, board);
}
