#pragma once
#include <cassert>
#include <iostream>
#include "board.h"
#include "piece-type.h"
#include "squares.h"

namespace piece {

    using sqrs = board::bitmap::Squares;

    struct Positions {
        sqrs all_armies;
        sqrs hostile_armies;
    };

    class Piece final {
        public:
            using update_fn = void (*)(piece::Piece& piece, const board::Board& board, const Positions& positions);

        private:
            update_fn update;
            bool was_moved{false};

        public:
            Piece(PieceType ptype, board::bitmap::Squares pos, update_fn _fn)
                : update{_fn}, type{ptype}, position{pos} {};
            Piece()
                : Piece(PieceType::_INVALID, 0,
                        [](piece::Piece&, const board::Board&, const piece::Positions&) {}) {};

            PieceType type{PieceType::_INVALID};
            board::bitmap::Squares position{0};
            board::bitmap::Squares observed{0};   // changes on these squares require update of piece
            board::bitmap::Squares attackable{0}; // squares which can be attacked by the piece
            board::bitmap::Squares movable{0};    // squares to which the piece can move to
 
            void update_observed_and_attackable(const board::Board&, sqrs,sqrs);
            void mark_as_dead();
            void move(board::bitmap::Squares dest);
            bool is_alive() const;
            bool has_moved() const;
            bool operator==(const Piece& other) const;
    };

}  // namespace piece
