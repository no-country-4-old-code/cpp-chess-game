#pragma once
#include <cassert>
#include <iostream>
#include "board.h"
#include "piece-type.h"
#include "squares.h"

namespace piece
{

    using sqrs = board::bitmap::Squares;

    struct Positions
    {
        sqrs all_armies;
        sqrs hostile_armies;
    };

    class Piece final
    {
    private:
        bool was_moved{false};
    
    public:
        using update_fn_ptr = void (*)(piece::Piece &piece, const board::Board &board, const Positions &positions);

        Piece(PieceType ptype, board::bitmap::Squares pos, update_fn_ptr _fn)
            : update_fn{_fn}, type{ptype}, position{pos} {};
        Piece()
            : Piece(PieceType::_INVALID, 0, [](piece::Piece &, const board::Board &, const piece::Positions &) {}) {};

        update_fn_ptr update_fn;
        PieceType type{PieceType::_INVALID};
        board::bitmap::Squares position{0};
        board::bitmap::Squares observed{0};   // changes on these squares require update of piece
        board::bitmap::Squares attackable{0}; // squares which can be attacked by the piece
        board::bitmap::Squares movable{0};    // squares to which the piece can move to

        void update(const board::Board &, sqrs, sqrs);
        void mark_as_dead();
        void move(board::bitmap::Squares dest);
        bool is_alive() const;
        bool has_moved() const;
        bool operator==(const Piece &other) const;
    };

} // namespace piece
