#include "player.h"
#include <iostream>
#include "color.h"

Color Player::color() const {
    return _army->color();
};

void Player::make_move() const {
    _ai->make_move(*_army);
};

bool Player::is_defeated() const {
    return _ai->is_defeated(*_army);
}
