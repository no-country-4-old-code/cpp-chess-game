#include "chaos.h"
#include <iostream>
#include <set>
#include "behaviour-ai.h"
#include "behaviour-human.h"
#include "pieces-color.h"
#include "player.h"
#include <board.h>
#include <aggregator-positions.h>
#include "piece-king.h"
#include "piece-rock.h"
#include "notation.h"

using PlayerGroup =
    std::set<Player, decltype([](const Player& a, const Player& b) {
                 return a.color() < b.color();
             })>;
static void run(PlayerGroup& group);

/**
 * Everything begins in Chaos
 */
void run_game() {
    board::Board board{8, 8};

    using namespace piece;
    using namespace board::notation::literal;

    piece::aggregator::army_list army_list = {
        piece::army::Army{Color::BLUE, {
            pieces::King{"d3"_n.as_squares(board)},
            pieces::Rock{"e4"_n.as_squares(board)},
            pieces::Rock{"b4"_n.as_squares(board)},
            pieces::Rock{"b6"_n.as_squares(board)},                        
        }},
        piece::army::Army{Color::WHITE, {
            pieces::King{"d1"_n.as_squares(board)},
            pieces::Rock{"f8"_n.as_squares(board)},
            pieces::Rock{"g7"_n.as_squares(board)},
            pieces::Rock{"h6"_n.as_squares(board)},                        
        }},
        piece::army::Army{Color::ORANGE, {
            pieces::King{"d5"_n.as_squares(board)},
            pieces::Rock{"d4"_n.as_squares(board)}
        }
        },
        piece::army::Army{}
    };

    PlayerBehaviourAI a{};
    PlayerGroup group{
        Player{&a, army_list[0]},
        Player{&a, army_list[1]},
        Player{&a, army_list[2]},
    };
    run(group);
}

static void run(PlayerGroup& group) {
    unsigned int turn = 1;

    while (group.size() > 1) {
        std::cout << "Turn " << turn << "\n";

        for (auto player = group.begin(); player != group.end();) {
            if (player->has_valid_moves()) {
                player->make_move();
                ++player;
            } else {
                std::cout << ">> Remove Player " << player->color() << "\n";
                player = group.erase(player);
                if (group.size() <= 1) {
                    break;  // game over
                }
            }
        };

        ++turn;
    };
    std::cout << "Player " << group.begin()->color() << " wins !\n";
}
