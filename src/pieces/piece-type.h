#pragma once
#include <cstdlib>

namespace piece
{

    enum class PieceType : u_int8_t
    {
        KING,
        QUEEN,
        KNIGHT,
        BISHOP,
        ROCK,
        PEASANT,
        _COUNT,
        _INVALID
    };

}
