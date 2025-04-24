#pragma once
#include "piece-update.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "notation.h"
#include "board-movements.h"
#include "pieces-color.h"
#include "aggregator-positions.h"
#include <array>
#include <iostream>

namespace piece::update::attackable {
    using sqrs =  board::bitmap::Squares;

    void update_piece(piece::Piece& piece, const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies);

}
