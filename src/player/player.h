#pragma once
#include "chess-ai.h"
#include "color.h"

class Player
{
private:
    Color _color;
    ChessAI *_behaviour;
    piece::army::Army &_army;

public:
    explicit Player(ChessAI *behaviour, piece::army::Army &army)
        : _color{army.color()}, _behaviour{behaviour}, _army{army} {}
    [[nodiscard]] Color color() const;
    void make_move() const;
    bool is_defeated() const;
};
