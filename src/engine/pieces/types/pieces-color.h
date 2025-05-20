#pragma once
#include <cstdint>
#include <ostream>

enum class Color : std::uint8_t
{
    WHITE,
    BLACK,
    ORANGE,
    BLUE,
    _COUNT,
    _INVALID
};

std::ostream &operator<<(std::ostream &out, Color color);
