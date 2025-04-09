#include "chaos.h"
#include <behaviour.h>
#include <pieces-color.h>
#include <player.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

static void run(std::vector<Player>& group);
static void remove_player(Player& player, std::vector<Player>& group);

/**
 * Everything begins in Chaos
 */
void run_game() {
    PlayerBehaviourAI a{};
    PlayerBehaviourHuman b{};
    std::vector<Player> group{
        Player{Color::WHITE, a},
        Player{Color::BLACK, b},
        Player{Color::ORANGE, b},
        Player{Color::BLUE, b},
    };
    run(group);
}

static void run(std::vector<Player>& group) {
    unsigned int turn = 1;

    while (group.size() > 1) {
        std::cout << "Turn " << turn << "\n";

        for (auto player : group) {
            if (player.has_valid_moves()) {
                player.make_move();
            } else {
                remove_player(player, group);
                if (group.size() <= 1) {
                    break; // game over
                }
            }
        };

        ++turn;
    };
    std::cout << "Player " << group[0].color() << " wins !\n";
}

static void remove_player(Player& player, std::vector<Player>& group) {
    auto color = player.color();
    std::cout << "Remove Player " << color << "\n";
    auto begin_of_deleted = std::remove_if(
        group.begin(), group.end(),
        [&color](const Player& p) { return p.color() == color; });
    group.erase(begin_of_deleted, group.end());
};
