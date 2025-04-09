#include "behaviour.h"
#include <iostream>

IPlayerBehaviour::~IPlayerBehaviour() {};

void PlayerBehaviourHuman::make_move() const {
    std::cout << "<HUMAN>";
}

void PlayerBehaviourAI::make_move() const {
    std::cout << "<AI>";
}