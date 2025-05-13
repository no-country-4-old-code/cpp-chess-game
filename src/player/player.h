#pragma once
#include "interface-player-behaviour.h"
#include "pieces-color.h"
#include <aggregator-positions.h>

class Player
{
private:
    Color _color;
    IPlayerBehaviour *_behaviour;
    piece::army::Army &_army;

public:
    explicit Player(IPlayerBehaviour *behaviour, piece::army::Army &army)
        : _color{army.color()}, _behaviour{behaviour}, _army{army} {}
    [[nodiscard]] Color color() const;
    void make_move() const;
    bool is_defeated() const;
};
