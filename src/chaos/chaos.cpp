#include "chaos.h"
#include <player.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <set>


using PlayerGroup = std::set<Player, decltype([](const Player& a, const Player& b) {
    return a.color() < b.color();
})>;
static void run(PlayerGroup& group);

/**
 * Everything begins in Chaos
 */
void run_game() {
    PlayerBehaviourAI a{};
    PlayerBehaviourHuman b{};
    PlayerGroup group{
        Player{Color::WHITE, &a},
        Player{Color::BLACK, &b},
        Player{Color::ORANGE, &b},
        Player{Color::BLUE, &b},
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
                    break; // game over
                }
            }
        };

        ++turn;
    };
    std::cout << "Player " << group.begin()->color() << " wins !\n";
}
