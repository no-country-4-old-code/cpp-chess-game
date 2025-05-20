#include "player.h"
#include "color.h"
#include <iostream>

Color Player::color() const
{
    return _color;
};

void Player::make_move() const
{
    _behaviour->make_move(_army);
    // std::cout << " > " << this->color() << " makes a move\n";
};

bool Player::is_defeated() const
{
    return _behaviour->is_defeated(_army);
}
