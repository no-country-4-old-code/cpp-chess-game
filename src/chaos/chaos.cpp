#include "chaos.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>

enum class Color : std::uint8_t { WHITE, BLACK, COUNT };
static std::ostream& operator<<(std::ostream& out, Color color);

static void print_turn_info(unsigned int iteration,
                            unsigned int number_of_players);

class Player {
    const Color _color;  // NOLINT // copy assignment not needed
   public:
    explicit Player(const Color color) : _color{color} {
    }

    [[nodiscard]] Color color() const;
    void  make_move() const;
};

Color Player::color() const {
    return _color;
}

void Player::make_move() const {
    std::cout << "> " << _color << " makes a move\n";
}

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
    IteratorCirulating<Players> player(players.begin(), players.end());

    std::cout << "Start game";
    unsigned int iteration = 0;

    while (is_game_ongoing(iteration)) {
        print_turn_info(iteration, players.size());

        player->make_move();

        ++iteration;
        ++player;
    };
}

static void print_turn_info(unsigned int iteration,
                            unsigned int number_of_players) {
    auto turn = (iteration / number_of_players) + 1;
    std::cout << "Turn " << turn;
}
