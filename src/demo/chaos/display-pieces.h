#pragma once
#include "board.h"
#include "army.h"

namespace display
{
    void display_all_pieces(const board::Board &board, const piece::army::army_list &army_list);
}

std::ostream &operator<<(std::ostream &out, Color color);
std::ostream &operator<<(std::ostream &out, const piece::PieceType& piece);