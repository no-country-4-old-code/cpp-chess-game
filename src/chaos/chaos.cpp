#include "chaos.h"
#include <iostream>
#include <set>
#include "behaviour-ai.h"
#include "pieces-color.h"
#include "player.h"
#include <board.h>

#include "piece-king.h"
#include "piece-rock.h"
#include "piece-bishop.h"
#include "notation.h"
#include "piece-api.h"

using PlayerGroup =
    std::set<Player, decltype([](const Player &a, const Player &b)
                              { return a.color() < b.color(); })>;
static void run(PlayerGroup &group);

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
                                           pieces::King{"a1"_n.as_squares(board)},
                                           pieces::Rock{"e4"_n.as_squares(board)},
                                           pieces::Rock{"b4"_n.as_squares(board)},
                                           pieces::Bishop{board, "b6"_n},
                                       }},
        piece::army::Army{Color::WHITE, {
                                            pieces::King{"h1"_n.as_squares(board)},
                                            pieces::Rock{"f8"_n.as_squares(board)},
                                            pieces::Rock{"g7"_n.as_squares(board)},
                                            pieces::Rock{"h6"_n.as_squares(board)},
                                        }},
        piece::army::Army{Color::ORANGE, {pieces::King{"d8"_n.as_squares(board)}, pieces::Rock{"d7"_n.as_squares(board)}}}, piece::army::Army{}};

    piece::api::init_army_list(army_list, board);

    PlayerBehaviourAI a{army_list, board};
    PlayerGroup group{
        Player{&a, army_list[0]},
        Player{&a, army_list[1]},
        Player{&a, army_list[2]},
    };
    run(group);
}

static void run(PlayerGroup &group)
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
                ++player;
            }
        };

        ++turn;
    };
    std::cout << "Player " << group.begin()->color() << " wins !\n";
}
