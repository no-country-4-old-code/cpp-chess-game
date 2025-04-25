#include "piece-king.h"
#include "piece.h"
#include "board-movements.h"

namespace piece::pieces {
    namespace move = board::movements;

    void King::update_observed_and_attackable(const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies) {
        const auto& position = this->position;
        this->observed = 
        move::left(position, board) | 
        move::left_up(position, board) | 
        move::up(position, board) | 
        move::right_up(position, board) | 
        move::right(position, board) | 
        move::right_down(position, board) |
        move::down(position, board) | 
        move::left_down(position, board);

        auto pos_own_pieces = pos_all & ~pos_hostile_armies;
        this->attackable = this->observed & ~pos_own_pieces; // can not attack own pieces
    }
}