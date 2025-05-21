#pragma once
#include "chess-ai.h"
#include "color.h"

class Player
{
private:
    ChessAI *_ai;
    piece::army::Army *_army;

public:
    explicit Player(ChessAI *ai, piece::army::Army *army): _ai{ai}, _army{army} {}
    [[nodiscard]] Color color() const;
    void make_move() const;
    bool is_defeated() const;
};
