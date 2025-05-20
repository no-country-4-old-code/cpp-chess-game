#pragma once
#include <cstdlib>
#include <ostream>

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

    std::ostream &operator<<(std::ostream &out, const piece::PieceType& piece);
}
