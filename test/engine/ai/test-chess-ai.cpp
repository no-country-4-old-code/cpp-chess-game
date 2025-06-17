#include <gtest/gtest.h>
#include <notation.h>
#include "test-helper.h"
#include "piece-actions.h"
#include "pieces.h"
#include "chess-ai.h"

using namespace board::notation::literal;
using namespace piece;

namespace {
    const board::Board default_board{8, 8};

    piece::army::army_list create_army_list(board::Board board, 
                                            const std::vector<piece::Piece>& white,
                                            const std::vector<piece::Piece>& black) {
        piece::army::army_list army_list = {piece::army::Army{Color::WHITE, white},
                                            piece::army::Army{Color::BLACK, black}};
        piece::api::init_army_list(army_list, board);
        return army_list;
    }
}

// START make_move tests

TEST(ChessAIMakeMove, CheckmateEnemyInOneMove) {
    board::Board board{4, 4};
    auto army_list = create_army_list(board, 
        {King(board, "b3"_n), Rock(board, "c3"_n)},
        {King(board, "a1"_n), Rock(board, "d4"_n)}
    );
    auto ai = ChessAI(army_list, board);
    auto move = ai.make_move(army_list[0]);
    EXPECT_EQ(move.src, "c3"_n.as_squares(board));  // move rock to checkmate king on a1
    EXPECT_EQ(move.dest, "c1"_n.as_squares(board));
    EXPECT_EQ(move.extra.src, 0); // no extra move (like in casteling etc.)
}


// END make_move tests
// START defeated tests

TEST(ChessAIDefeated, DefeatedIfOnlyKingAlive) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {King(board, "a1"_n)},
        {King(board, "b3"_n), Rock(board, "b2"_n)}
    );
    auto ai = ChessAI(army_list, board);
    EXPECT_TRUE(ai.is_defeated(army_list[0]));    
}

TEST(ChessAIDefeated, DefeatedIfNoKingAlive) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {King(board, "a1"_n), Rock(board, "a2"_n), Rock(board, "a3"_n)},
        {King(board, "b3"_n), Rock(board, "b2"_n)}
    );
    army_list[0].pieces[0].mark_as_dead();
    auto ai = ChessAI(army_list, board);
    EXPECT_TRUE(ai.is_defeated(army_list[0]));    
}

TEST(ChessAIDefeated, DefeatedIfNoMovesLeft) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {King(board, "a1"_n), Rock(board, "a2"_n), Rock(board, "a3"_n)},
        {King(board, "b3"_n), Rock(board, "b2"_n), Rock(board, "b1"_n)}
    );
    auto ai = ChessAI(army_list, board);
    EXPECT_TRUE(ai.is_defeated(army_list[0]));    
}

TEST(ChessAIDefeated, NotDefeated) {
    board::Board board{3, 3};
    auto army_list = create_army_list(board, 
        {King(board, "a1"_n), Rock(board, "a2"_n), Rock(board, "a3"_n)},
        {King(board, "b3"_n), Rock(board, "b2"_n)}
    );
    auto ai = ChessAI(army_list, board);
    EXPECT_FALSE(ai.is_defeated(army_list[0]));    
}

// END defeated tests
