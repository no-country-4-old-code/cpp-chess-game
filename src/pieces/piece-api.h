#pragma once
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "aggregator-positions.h"
#include "stack-vector.h"
#include "army.h"

namespace piece::api {

    
    struct PieceDestinations {
        const piece::Piece* piece;
        board::bitmap::Squares destinations; // plural
    };


    using ArmyDestinations = StackVector<PieceDestinations, piece::army::max_pieces_per_army>;

    
    void move_piece(piece::Piece&, board::bitmap::Squares, const board::Board&, piece::aggregator::army_list&);
    ArmyDestinations calc_possible_moves(const piece::army::Army&, const board::Board&, const piece::aggregator::army_list&);
    void init_army_list(piece::aggregator::army_list& army_list, const board::Board& board);
}
