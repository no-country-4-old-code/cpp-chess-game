#include "run-game.h"
#include <board.h>
#include <iostream>
#include "army.h"
#include "display-board.h"
#include "piece-actions.h"
#include "player.h"

namespace demo::game {

    void run(PlayerGroup &group, const board::Board &board, piece::army::army_list &army_list) {
        unsigned int turn = 1;
        piece::api::init_army_list(army_list, board);

        while (group.size() > 1) {
            std::cout << "Turn " << turn << "\n";

            for (auto player = group.begin(); player != group.end();) {
                if (player->is_defeated()) {
                    std::cout << ">> Remove Player " << player->color() << "\n";
                    player = group.erase(player);
                    if (group.size() <= 1) {
                        break;  // game over
                    }
                } else {
                    player->make_move();
                    display::display_board(board, army_list);
                    ++player;
                }
            };

            ++turn;
        };
        std::cout << "Player " << group.begin()->color() << " wins !\n";
    }
}  // namespace demo::game
