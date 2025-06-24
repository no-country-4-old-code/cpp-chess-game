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
    const auto down = MoveDirection::DOWN;
    const auto up = MoveDirection::UP;
    const board::Board b{8, 8};

    piece::army::army_list create_army_list(board::Board board, 
                                            const std::vector<piece::Piece>& white,
                                            const std::vector<piece::Piece>& black) {
        piece::army::army_list army_list = {piece::army::Army{Color::WHITE, white},
                                            piece::army::Army{Color::BLACK, black}};
        piece::api::init_army_list(army_list, board);
        return army_list;
    }

}


TEST(ChessAIChessRiddle, CheckmateInTwo) {
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
    auto ai = ChessAI(army_list, b);
    //display::display_board(b, army_list);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move = ai.make_move(army_list[1]);

    EXPECT_EQ(move.src, 0);  // checkmated
    EXPECT_EQ(move.dest, 0);
}

TEST(ChessAIChessRiddle, CheckmateInTwo_2) {
    /*
         a   b   c   d   e   f   g   h 

1                                    Kb 
2                                    Pw 
3                            Kw         
4                                       
5                Qw                  Nb 
6                                       
7                            Rw         
8                                                                    
    */
    auto army_list = create_army_list(b, 
        {
            Queen(b, "c5"_n),
            Pawn(b, "h2"_n, down),
            King(b, "f3"_n), Rock(b, "f7"_n)
        },
        {
            Knight(b, "h5"_n),
            King(b, "h1"_n)
        }
    );
    auto ai = ChessAI(army_list, b);
    //display::display_board(b, army_list);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move = ai.make_move(army_list[1]);

    EXPECT_EQ(move.src, 0);  // checkmated
    EXPECT_EQ(move.dest, 0);
}


TEST(ChessAIChessRiddle, CheckmateInTwo_3) {
    /*
         a   b   c   d   e   f   g   h 

1                        Kw          Qw 
2                                       
3                                       
4                Rw      Rw      Nb     
5                    Kb                 
6                                       
7                                       
8                                       
    
    */
    auto army_list = create_army_list(b, 
        {
            King(b, "e1"_n), Queen(b, "h1"_n),
            Rock(b, "e4"_n), Rock(b, "c4"_n)
        },
        {
            Knight(b, "g4"_n), 
            King(b, "d5"_n)
        }
    );
    //display::display_board(b, army_list);
    auto ai = ChessAI(army_list, b);
    ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move = ai.make_move(army_list[1]);

    EXPECT_EQ(move.src, 0);  // checkmated
    EXPECT_EQ(move.dest, 0);
}

TEST(ChessAIChessRiddle, CheckmateInThree_1) {
    /*
         a   b   c   d   e   f   g   h 

1                                    Bw 
2                                       
3                                       
4                                       
5                                       
6        Nw                             
7        Pb  Pb      Kw                 
8        Kb                                               
    */
    auto army_list = create_army_list(b, 
        {
            Bishop(b, "c6"_n),
            Knight(b, "a6"_n), King(b, "d7"_n),
        },
        {
            King(b, "a8"_n), 
            Pawn(b, "a7"_n, up), Pawn(b, "b7"_n, up),
        }
    );
    /*
    auto army_list = create_army_list(b, 
        {
            Bishop(b, "h1"_n),
            Knight(b, "a6"_n), King(b, "d7"_n),
        },
        {
            King(b, "a8"_n), 
            Pawn(b, "a7"_n, up), Pawn(b, "b7"_n, up),
        }
    );
    */
    auto ai = ChessAI(army_list, b);
    display::display_board(b, army_list);
    std::cout << army_list[0].pieces[3].attackable << std::endl;
    //ai.make_move(army_list[0]);
    ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move1 = ai.make_move(army_list[1]);
    ai.make_move(army_list[0]);
    auto move2 = ai.make_move(army_list[1]);

    EXPECT_GT(move1.src, 0);  // not checkmated
    EXPECT_GT(move1.dest, 0);

    EXPECT_EQ(move2.src, 0);  // checkmated
    EXPECT_EQ(move2.dest, 0);
}
