#pragma once
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "aggregator-positions.h"

namespace piece::api {
    
    void move_piece(piece::Piece&, board::bitmap::Squares, const board::Board&, piece::aggregator::army_list&);
    void init_army_list(piece::aggregator::army_list& army_list, const board::Board& board);
}
