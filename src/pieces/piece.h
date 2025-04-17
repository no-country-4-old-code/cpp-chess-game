#pragma once
#include "pieces-color.h"
#include "squares.h"

namespace piece {
    using PieceType = u_int8_t;

    struct Piece {
            Color color;
            PieceType type;
            board::bitmap::Squares position;

            // All "seen" squares and pieces
            board::bitmap::Squares observed;
            // All squares the piece might move to (not considering checkmate)
            board::bitmap::Squares attackable;  
            // All squares the piece can move to (considering checkmate)
            board::bitmap::Squares movable; 
            
            Piece(Color color_, PieceType type_, board::bitmap::Squares pos_): color{color_}, type{type_}, position{pos_} {};
    };
}
