#pragma once
#include <cstdlib>

namespace piece {

    enum class PieceType: u_int8_t {
        INVALID,
        KING,
        QUEEN,
        KNIGHT,
        BISHOP,
        ROCK,
        PEASANT
    };
    
}

