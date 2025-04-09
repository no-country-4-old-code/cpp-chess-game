#include "chaos.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <pieces-color.h>
#include <player.h>
#include <vector>
#include <algorithm>



class PlayerBehaviour {
    public:
        virtual void make_move() const = 0;
        virtual ~PlayerBehaviour()     = 0;
};

PlayerBehaviour::~PlayerBehaviour() {};

class PlayerBehaviourHuman : public PlayerBehaviour {
    public:
        void make_move() const override;
};

void PlayerBehaviourHuman::make_move() const {
    std::cout << "<HUMAN>";
}

class PlayerBehaviourAI : public PlayerBehaviour {
    public:
        void make_move() const override;
};

void PlayerBehaviourAI::make_move() const {
    std::cout << "<AI>";
}

class Player {
    private:
        Color _color;  // NOLINT // copy assignment not needed
        PlayerBehaviour* _behaviour;  // NOLINT

    public:
        explicit Player(const Color color, PlayerBehaviour& behaviour)
            : _color{color}, _behaviour{&behaviour} {}
        [[nodiscard]] Color color() const;
        void make_move() const;
        bool has_valid_moves() const;
};

Color Player::color() const {
    return _color;
};

void Player::make_move() const {
    _behaviour->make_move();
    std::cout << " > " << this->color() << " makes a move\n";
};

bool Player::has_valid_moves() const {
    // get_available_moves
    static unsigned int i = 20 + static_cast<int>(this->color());
    --i;
    if (i == 0) {
        i = 20;
        return false;
    } else {
        return true;
    }
}



void remove_player( Player& player, std::vector<Player>& group) {
    auto color = player.color();  
    std::cout << "Remove Player " << color << "\n";
    auto begin_of_deleted = std::remove_if(
        group.begin(), 
        group.end(), 
        [&color] (const Player& p) {return p.color() == color; }
    );
    group.erase(begin_of_deleted, group.end());
};



void run(std::vector<Player>& group) {
    unsigned int turn = 1;

    while (group.size() > 1) {
        std::cout << "Turn " << turn << "\n";
        
        for(auto player : group) {
            if (player.has_valid_moves()) {
                player.make_move();
            } else {
                remove_player(player, group);
                if (group.size() <= 1) {
                    break;
                }
            }
        };

        ++turn;
    };
    std::cout << "Player " << group[0].color() << " wins !\n";
}

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
