#pragma once
#include "army.h"
#include "board.h"

namespace display {
    void display_board(const board::Board &board, const piece::army::army_list &army_list);
}

std::ostream &operator<<(std::ostream &out, Color color);
std::ostream &operator<<(std::ostream &out, const piece::PieceType &piece);