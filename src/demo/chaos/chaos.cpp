#include "chaos.h"
#include <iostream>
#include <set>
#include "color.h"
#include "player.h"
#include "chess-ai.h"
#include <board.h>
#include "display-pieces.h"
#include "pieces.h"
#include "pieces.h"
#include "pieces.h"
#include "notation.h"
#include "piece-actions.h"

using PlayerGroup =
    std::set<Player, decltype([](const Player &a, const Player &b)
                              { return a.color() < b.color(); })>;

namespace
{
    void run(PlayerGroup &, const board::Board &, const piece::army::army_list &);
}

/**
 * Everything begins in Chaos
 */
void run_game()
{
    board::Board board{8, 8};

    using namespace piece;
    using namespace board::notation::literal;

    piece::army::army_list army_list = {
        piece::army::Army{Color::BLUE, {
                                           King{"a1"_n.as_squares(board)},
                                           Rock{"e4"_n.as_squares(board)},
                                           Rock{"b4"_n.as_squares(board)},
                                           Bishop{board, "b6"_n},
                                       }},
        piece::army::Army{Color::WHITE, {
                                            King{"h1"_n.as_squares(board)},
                                            Rock{"f8"_n.as_squares(board)},
                                            Rock{"g7"_n.as_squares(board)},
                                            Rock{"h6"_n.as_squares(board)},
                                        }},
        piece::army::Army{Color::ORANGE, {King{"d8"_n.as_squares(board)}, Rock{"d7"_n.as_squares(board)}}}, piece::army::Army{}};

    piece::api::init_army_list(army_list, board);

    ChessAI a{army_list, board};
    PlayerGroup group{
        Player{&a, army_list[0]},
        Player{&a, army_list[1]},
        Player{&a, army_list[2]},
    };
    run(group, board, army_list);
}

namespace
{
    void run(PlayerGroup &group, const board::Board &board, const piece::army::army_list &army_list)
    {
        unsigned int turn = 1;

        while (group.size() > 1)
        {
            std::cout << "Turn " << turn << "\n";

            for (auto player = group.begin(); player != group.end();)
            {
                if (player->is_defeated())
                {
                    std::cout << ">> Remove Player " << player->color() << "\n";
                    player = group.erase(player);
                    if (group.size() <= 1)
                    {
                        break; // game over
                    }
                }
                else
                {
                    player->make_move();
                    display::display_all_pieces(board, army_list);
                    ++player;
                }
            };

            ++turn;
        };
        std::cout << "Player " << group.begin()->color() << " wins !\n";
    }
}
