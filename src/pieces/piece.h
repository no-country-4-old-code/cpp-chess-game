#pragma once
#include "squares.h"
#include "piece-type.h"

namespace piece {

    struct Piece {
            PieceType type;
            board::bitmap::Squares position;

            // All "seen" squares and pieces
            board::bitmap::Squares observed;
            // All squares the piece might move to (not considering checkmate)
            board::bitmap::Squares attackable;  
            // All squares the piece can move to (considering checkmate)
            board::bitmap::Squares movable; 
            
            Piece(): Piece(PieceType::_INVALID, 0) {};
            Piece(PieceType ptype, board::bitmap::Squares pos): type{ptype}, position{pos} {};
    };
}
