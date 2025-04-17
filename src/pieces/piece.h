#pragma once
#include "pieces-color.h"
#include "squares.h"

namespace piece {
    using PieceType = u_int8_t;

    struct Piece {
            Color color;
            PieceType type;
            board::bitmap::Squares position;

            board::bitmap::Squares
                observed;  // All "seen" squares including ones blocked by other
                           // pieces (including own)
            board::bitmap::Squares
                attackable;  // All squares the piece might move to (not
                             // considering checkmate)
            board::bitmap::Squares movable;  // All squares the piece can move
                                             // to (considering checkmate)
    };
}
