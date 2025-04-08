#include "chaos.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>

enum class Color : std::uint8_t { WHITE, BLACK, COUNT };

class Player {
   public:
    const Color color;  // NOLINT
    explicit Player(const Color _color) : color{_color} {
    }
};

static bool is_game_ongoing(unsigned int iteration) {
    return iteration < 10;  // NOLINT
};

static const std::array<std::string_view, static_cast<size_t>(Color::COUNT)>
    lookup_color_name{"White", "Black"};

static std::ostream& operator<<(std::ostream& out, const Color color) {
    auto index = static_cast<size_t>(color);
    if (index < lookup_color_name.size()) {
        out << lookup_color_name[index];  // NOLINT (*-pro-bounds-*) // bounds
                                          // are checked
    } else {
        out << "Unknown";
    }
    return out;
};

template <typename CONTAINER>
class IteratorCirulating {
    using Iterator = CONTAINER::iterator;
    Iterator _begin, _end, _current;

   public:
    IteratorCirulating(Iterator begin, Iterator end)
        : _begin(begin), _end(end), _current(begin) {};

    IteratorCirulating& operator++() {
        ++_current;
        if (_current == _end) {
            _current = _begin;
        }
        return *this;
    };

    auto operator->() {
        return _current;
    };
};

/**
 * Everything begins in Chaos
 */
void run_game() {
    using Players = std::array<Player, 2>;
    Players players{Player{Color::WHITE}, Player{Color::BLACK}};
    IteratorCirulating<Players> current_player(players.begin(), players.end());

    unsigned int iteration = 0;

    std::cout << "Run game";
    while (is_game_ongoing(iteration)) {
        auto turn = iteration / players.size() + 1;
        std::cout << "Turn " << turn << " - Player " << current_player->color
                  << "\n";

        ++iteration;
        ++current_player;
    };
}