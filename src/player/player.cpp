#include "player.h"
#include <iostream>

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

void PlayerBehaviourHuman::make_move() const {
    std::cout << "<HUMAN>";
}

bool PlayerBehaviourHuman::has_valid_moves() const {
    return true;
}

void PlayerBehaviourAI::make_move() const {
    std::cout << "<AI>";
}


bool PlayerBehaviourAI::has_valid_moves() const {
    return true;
}
