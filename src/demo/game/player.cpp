#include "player.h"
#include "color.h"
#include <iostream>

Color Player::color() const
{
    return _army->color();
};

void Player::make_move() const
{
    ai->make_move(*_army);
};

bool Player::is_defeated() const
{
    return ai->is_defeated(*_army);
}
