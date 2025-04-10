#include "behaviour-human.h"
#include <iostream>

void PlayerBehaviourHuman::make_move() const {
    std::cout << "<HUMAN>";
}

bool PlayerBehaviourHuman::has_valid_moves() const {
    return true;
}
