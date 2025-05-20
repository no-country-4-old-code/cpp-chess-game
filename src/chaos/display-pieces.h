#pragma once
#include "board.h"
#include "army.h"

namespace display
{
    void display_all_pieces(const board::Board &board, const piece::army::army_list &army_list);
}
