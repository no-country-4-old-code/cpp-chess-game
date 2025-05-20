#include "piece-king.h"
#include "piece-baseclass.h"
#include "board-movements.h"

namespace piece::pieces::king
{
    namespace move = board::movements;

    void update_observed_and_attackable(piece::Piece &piece, const board::Board &board, sqrs pos_all, sqrs pos_hostile_armies)
    {
        const auto &position = piece.position;
        piece.observed =
            move::left(position, board) |
            move::left_up(position, board) |
            move::up(position, board) |
            move::right_up(position, board) |
            move::right(position, board) |
            move::right_down(position, board) |
            move::down(position, board) |
            move::left_down(position, board);

        auto pos_own_pieces = pos_all & ~pos_hostile_armies;
        piece.attackable = piece.observed & ~pos_own_pieces; // can not attack own pieces
    }
}