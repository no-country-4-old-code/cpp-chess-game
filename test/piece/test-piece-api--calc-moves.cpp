#include <gtest/gtest.h>
#include "piece-api.h"
#include <board.h>
#include <aggregator-positions.h>
#include "piece-king.h"
#include "piece-rock.h"
#include "notation.h"
#include "piece-api.h"
#include "piece-update.h"
#include "iterator-bitfield.h"
#include <iostream>

using namespace piece;
using namespace board::notation::literal;

namespace {
    void list_squares(const board::bitmap::Squares squares,const board::Board& board) {
        IteratorBitmap iter{squares};
        std::cout << "Squares: ";
        while (*iter) {
            std::cout << board::notation::ChessNotation{*iter, board} << ", ";
            ++iter;
        }
        std::cout << "\n" << std::flush;
    }

    piece::aggregator::army_list create_army_list(board::Board board, std::vector<piece::Piece> white, std::vector<piece::Piece> black) {
        piece::aggregator::army_list army_list = {
            piece::army::Army{Color::WHITE, white},
            piece::army::Army{Color::BLACK, black}
        };
        piece::api::init_army_list(army_list, board);
        return army_list;
    }
}

TEST(PieceApi_CalcMove, KingMovementObstructed) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {pieces::King{board, "b1"_n}}, 
        {pieces::King{board, "b3"_n}} );
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, "a1"_n.as_squares(board) | "c1"_n.as_squares(board));
}

TEST(PieceApi_CalcMove, KingMovementUnderAttack) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {pieces::King{board, "b1"_n}}, 
        {pieces::King{board, "c3"_n}, pieces::Rock{board, "b3"_n}} );
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].src, "b1"_n.as_squares(board));
    EXPECT_EQ(moves_all[0].destinations, "a1"_n.as_squares(board) | "c1"_n.as_squares(board) | "a2"_n.as_squares(board));
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, KingNoMovementPossible) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {pieces::King{board, "a1"_n}}, 
        {pieces::King{board, "c3"_n}, pieces::Rock{board, "b3"_n}, pieces::Rock{board, "c2"_n}} );
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 0);
}

TEST(PieceApi_CalcMove, KingCheckmate) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {pieces::King{board, "a1"_n}}, 
        {pieces::King{board, "c3"_n}, pieces::Rock{board, "b3"_n}, pieces::Rock{board, "a3"_n}} );
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 0);
    list_squares(moves_all[0].destinations, board);
}

TEST(PieceApi_CalcMove, KingOnlyMoveIsAttack) {
    board::Board board{2, 3};
    auto army_list = create_army_list(board, 
        {pieces::King{board, "a1"_n}}, 
        {pieces::King{board, "b2"_n}} );
    // act
    auto moves_all = piece::api::calc_possible_moves(army_list[0], board, army_list);
    // assert
    EXPECT_EQ(moves_all.size(), 1); // only one piece should move
    EXPECT_EQ(moves_all[0].destinations, "b2"_n.as_squares(board));
}