#include <array>
#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"

namespace
{
    namespace move = board::movements;

    template <move::move_func Move>
    void move_until_blocked(piece::Piece &piece, const board::Board &board, const piece::Positions &positions) {
        auto current = piece.position;
        while (current != 0)
        {
            current = Move(current, board);
            piece.observed |= current;

            if ((current & positions.all_armies))
            {
                if ((current & positions.hostile_armies))
                {
                    // only pieces of enemies can be attacked
                    piece.attackable |= current;
                }
                break;
            } 
            // squares in sight without a piece can be attacked
            piece.attackable |= current;
        }
    }


    template <move::move_func... Funcs>
    void update_template(piece::Piece &piece, const board::Board &board, const piece::Positions &positions) {
        piece.observed = 0;
        piece.attackable = 0;
        (move_until_blocked<Funcs>(piece, board, positions) , ...);
        piece.movable = piece.attackable;
    }


}

namespace piece
{
    Piece Rock(board::Board board, board::notation::ChessNotation notation)
    {
        return {
            PieceType::ROCK,
            notation.as_squares(board),
            ::update_template<move::left, move::right, move::up, move::down>
        };
    }
}

