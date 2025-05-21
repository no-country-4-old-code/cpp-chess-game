#pragma once
#include "squares.h"
#include "piece-type.h"
#include "board.h"
#include <cassert>
#include <iostream>


namespace piece {

    using sqrs =  board::bitmap::Squares;
    
    class Piece final {
        public:
            using update_fn = void (*)(piece::Piece& piece, const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies);

        private:
            update_fn update;
            
        public:
            Piece(PieceType ptype, board::bitmap::Squares pos, update_fn _fn): update{_fn}, type{ptype}, position{pos} {};
            Piece(): Piece(PieceType::_INVALID, 0, [](piece::Piece&, const board::Board&, sqrs, sqrs){}) {};

            PieceType type{PieceType::_INVALID};
            board::bitmap::Squares position{0};
            board::bitmap::Squares observed{0}; // = attackable & ~positions_of_friendly_pieces
            board::bitmap::Squares attackable{0}; // all squares the piece can move to (not considering checkmate)
            
            void update_observed_and_attackable(const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies);
            void mark_as_dead();
            bool is_alive() const;
            bool operator == (const Piece& other) const;    };

}
