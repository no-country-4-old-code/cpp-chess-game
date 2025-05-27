#pragma once
#include <cstdlib>
#include <ostream>

namespace piece {
    enum class PieceType : u_int8_t {
        KING,
        QUEEN,
        KNIGHT,
        BISHOP,
        ROCK,
        PAWN,
        _COUNT,
        _INVALID
    };
}
