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

        public:
            Piece(PieceType ptype, board::bitmap::Squares pos, update_fn _fn)
                : update{_fn}, type{ptype}, position{pos} {};
            Piece()
                : Piece(PieceType::_INVALID, 0,
                        [](piece::Piece&, const board::Board&, const piece::Positions&) {}) {};

            PieceType type{PieceType::_INVALID};
            board::bitmap::Squares position{0};
            board::bitmap::Squares observed{0};    // == attackable & ~positions_of_friendly_pieces
            board::bitmap::Squares attackable{0};

            void update_observed_and_attackable(const board::Board&, sqrs,sqrs);
            void mark_as_dead();
            bool is_alive() const;
            bool operator==(const Piece& other) const;
    };

}  // namespace piece
