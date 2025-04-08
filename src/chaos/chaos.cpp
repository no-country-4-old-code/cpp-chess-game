#include "chaos.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>

enum class Color : std::uint8_t { WHITE, BLACK, COUNT };
static std::ostream& operator<<(std::ostream& out, Color color);


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

class IteratorTurn {
    unsigned int _iteration{0};
    const std::uint8_t _group_size;
    public:
    IteratorTurn(const std::uint8_t group_size): _group_size{group_size} {}

    IteratorTurn& operator ++() {
        ++_iteration;
        return *this;
    }

    [[nodiscard]] unsigned int turn() {
        return (_iteration / _group_size) + 1;
    }
};

/**
 * Everything begins in Chaos
 */
void run_game() {
    using Players = std::array<Player, 2>;
    Players players{Player{Color::WHITE}, Player{Color::BLACK}};
    IteratorCirulating<Players> player(players.begin(), players.end());
    IteratorTurn turn_counter{players.size()};

    std::cout << "Start game\n";
    bool is_more_then_one_player_alive{true};

    while (is_more_then_one_player_alive) {
        std::cout << "Turn " << turn_counter.turn();;

        player->make_move();
        is_more_then_one_player_alive = turn_counter.turn() < 5; // NOLINT

        ++turn_counter;
        ++player;
    };
}
