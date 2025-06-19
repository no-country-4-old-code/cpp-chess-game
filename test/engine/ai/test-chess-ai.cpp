#include <gtest/gtest.h>
#include <notation.h>
#include "test-helper.h"
#include "piece-actions.h"
#include "pieces.h"
#include "chess-ai.h"
#include "display-board.h"

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
}

TEST(ChessAIMakeMove, PreventCheckmateInOneMove) {
    board::Board board{4, 4};
    auto army_list = create_army_list(board, 
        {King(board, "a1"_n), Rock(board, "d4"_n)},
        {King(board, "b3"_n), Rock(board, "c3"_n)}
    );
    auto ai = ChessAI(army_list, board);
    auto move = ai.make_move(army_list[0]);
    EXPECT_EQ(move.src, "a1"_n.as_squares(board));  // move king to prevent checkmate by rock on c1
    EXPECT_EQ(move.dest, "b1"_n.as_squares(board));
    EXPECT_EQ(move.extra.src, 0); // no extra move (like in casteling etc.)
}


TEST(ChessAIMakeMove, CheckmateEnemyInTwoMoves) {
    board::Board board{4, 4};
    auto army_list = create_army_list(board, 
        {King(board, "c4"_n), Rock(board, "d4"_n)},
        {King(board, "a1"_n)}
    );
    auto ai = ChessAI(army_list, board);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move = ai.make_move(army_list[1]);

    EXPECT_EQ(move.src, 0);  // checkmated
    EXPECT_EQ(move.dest, 0);
}

TEST(ChessAIMakeMove, CheckmateEnemyInThreeMoves) {
    board::Board board{4, 4};
    auto army_list = create_army_list(board, 
        {King(board, "c4"_n), Rock(board, "d4"_n)},
        {King(board, "b2"_n)}
    );
    auto ai = ChessAI(army_list, board);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move = ai.make_move(army_list[1]);

    EXPECT_EQ(move.src, 0);  // checkmated
    EXPECT_EQ(move.dest, 0);
}

TEST(ChessAIMakeMove, CheckmateEnemyInTwoMoves_std_board) {
    /*
         a   b   c   d   e   f   g   h 

1                    Qw                 
2                            Pw  Pw     
3                                       
4            Nw          Pw  Rw  Pb     
5            Kw  Pw      Kb      Rb     
6                        Pb  Bb  Nb     
7                        Pb             
8                                       
    */
    const auto down = MoveDirection::DOWN;
    const auto up = MoveDirection::UP;
    board::Board b{8, 8};
    auto army_list = create_army_list(b, 
        {
            Queen(b, "d1"_n),
            Pawn(b, "f2"_n, down), Pawn(b, "g2"_n, down),
            Knight(b, "b4"_n), Pawn(b, "e4"_n, down), Rock(b, "f4"_n), 
            King(b, "b5"_n), Pawn(b, "c5"_n, down)
        },
        {
            Pawn(b, "g4"_n, up),
            King(b, "e5"_n), Rock(b, "g5"_n), 
            Pawn(b, "e6"_n, up), Bishop(b, "f6"_n), Knight(b, "g6"_n),
            Pawn(b, "e7"_n, up),
        }
    );
    army_list[0].pieces[1].move("f2"_n.as_squares(b)); // quick hack  // TODO:: attackable == observable for Rock /etc. For other is observable == attackable & movable 
    // https://www.youtube.com/watch?v=D5hKsGix4dI
    auto ai = ChessAI(army_list, b);
    piece::api::init_army_list(army_list, b); // TODO : remove because quick hav
    display::display_board(b, army_list);
    std::cout << army_list[0].pieces[3].attackable << std::endl;
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move = ai.make_move(army_list[1]);

    EXPECT_EQ(move.src, 0);  // checkmated
    EXPECT_EQ(move.dest, 0);
}

TEST(ChessAIMakeMove, CheckmateEnemyInThreeMoves_std_board) {
    /*
         a   b   c   d   e   f   g   h 

1                        Rw  Kw         
2        Pw  Pw  Pw              Pw     
3                Kw  Pw          Rw     
4                                    Pw 
5                    Pb                 
6                Pb                  Qw 
7        Pb  Pb              Pb      Pb 
8        Rb  Kb      Qb          Rb  Kb 
    
    */
    const auto down = MoveDirection::DOWN;
    const auto up = MoveDirection::UP;
    board::Board b{8, 8};
    auto army_list = create_army_list(b, 
        {
            Rock(b, "e1"_n), King(b, "f1"_n),
            Pawn(b, "a2"_n, down), Pawn(b, "b2"_n, down), Pawn(b, "c2"_n, down), Pawn(b, "g2"_n, down),
            Knight(b, "c3"_n), Pawn(b, "d3"_n, down), Rock(b, "g3"_n),
            Pawn(b, "h4"_n, down),
            Queen(b, "h6"_n)
        },
        {
            Rock(b, "a8"_n), Knight(b, "b8"_n), Queen(b, "d8"_n), Rock(b, "g8"_n), 
            Pawn(b, "a7"_n, up), Pawn(b, "b7"_n, up), Pawn(b, "f7"_n, up), Pawn(b, "h7"_n, up),
            Pawn(b, "c6"_n, up), Pawn(b, "d5"_n, up), King(b, "h8"_n)
        }
    );
    display::display_board(b, army_list);
    auto ai = ChessAI(army_list, b);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move = ai.make_move(army_list[1]);

    EXPECT_EQ(move.src, 0);  // checkmated
    EXPECT_EQ(move.dest, 0);
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
