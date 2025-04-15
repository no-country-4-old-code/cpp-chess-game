#include "pieces-color.h"
#include <array>
#include <cstddef>
#include <ostream>
#include <string_view>

static const std::array<std::string_view, static_cast<size_t>(Color::COUNT)>
    lookup_color_name{"White", "Black", "Orange", "Blue"};

std::ostream& operator<<(std::ostream& out, const Color color) {
    auto index = static_cast<size_t>(color);
    if (index < lookup_color_name.size()) {
        out << lookup_color_name[index];  // NOLINT (*-pro-bounds-*) // bounds
                                          // are checked
    } else {
        out << "Unknown";
    }
    return out;
};
