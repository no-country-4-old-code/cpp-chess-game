#pragma once
#include "board.h"
#include "aggregator-positions.h"

namespace piece::update {
    
    void update_piece();
    void display_all_pieces(const board::Board& board, const piece::aggregator::army_list& army_list);
    void display_bits_on_board(const board::Board& board, board::bitmap::Squares sqrs);
}

