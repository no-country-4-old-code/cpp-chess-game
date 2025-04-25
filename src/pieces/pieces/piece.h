#pragma once
#include "squares.h"
#include "piece-type.h"
#include "board.h"

namespace piece {

    using sqrs =  board::bitmap::Squares;

    class Piece {
        protected:
            Piece(PieceType ptype, board::bitmap::Squares pos): type{ptype}, position{pos} {};
        public:
            PieceType type;
            board::bitmap::Squares position;

            // All "seen" squares and pieces
            board::bitmap::Squares observed;
            // All squares the piece might move to (not considering checkmate)
            board::bitmap::Squares attackable;  
            // All squares the piece can move to (considering checkmate)
            board::bitmap::Squares movable; 
            
            Piece(): Piece(PieceType::_INVALID, 0) {};
            virtual ~Piece()=default;
            virtual void update_observed_and_attackable(const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies) {
                // not pure virtual 
            };
    };
}
