#pragma once
#include <cstdint>

enum class Color : std::uint8_t { WHITE, BLACK, ORANGE, BLUE, COUNT };

std::ostream& operator<<(std::ostream& out, Color color);
