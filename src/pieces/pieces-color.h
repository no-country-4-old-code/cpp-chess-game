#pragma once
#include <cstdint>
#include <ostream>

enum class Color : std::uint8_t { INVALID, WHITE, BLACK, ORANGE, BLUE, COUNT };

std::ostream& operator<<(std::ostream& out, Color color);
