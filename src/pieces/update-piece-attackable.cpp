#include "update-piece-attackable.h"
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

namespace {
    using namespace piece::update::attackable;

    using update_fn = void (*)(piece::Piece& piece, const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies);
    using lookup_fn = std::array<update_fn, static_cast<size_t>(piece::PieceType::_COUNT)>;

    // update piece functions
    void update_king(piece::Piece &piece, const board::Board &board, sqrs pos_all, sqrs pos_hostile_armies);


    // lookup
    static const lookup_fn lookup {
        update_king

    };
}

namespace piece::update::attackable {
    
    void update_piece(piece::Piece& piece, const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies) {
        auto idx = static_cast<size_t>(piece.type);
        lookup[idx](piece, board, pos_all, pos_hostile_armies);
    }

}

namespace {
    namespace move = board::movements;

    void update_king(piece::Piece &piece, const board::Board &board, sqrs pos_all, sqrs pos_hostile_armies){
        
        piece.observed = 
            move::left(piece.position, board) | 
            move::left_up(piece.position, board) | 
            move::up(piece.position, board) | 
            move::right_up(piece.position, board) | 
            move::right(piece.position, board) | 
            move::right_down(piece.position, board) |
            move::down(piece.position, board) | 
            move::left_down(piece.position, board);

        auto pos_own_pieces = pos_all & ~pos_hostile_armies;
        piece.attackable = piece.observed & ~pos_own_pieces; // can not attack own pieces
    }
}
