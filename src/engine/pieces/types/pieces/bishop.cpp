#include <array>
#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"

namespace
{
    void update(piece::Piece &, const board::Board &, const piece::Positions &);
}

namespace piece
{
    Piece Bishop(board::Board board, board::notation::ChessNotation notation)
    {
        return {PieceType::BISHOP, notation.as_squares(board), ::update};
    }
}

namespace
{
    namespace move = board::movements;

    void update(piece::Piece &piece, const board::Board &board, const piece::Positions &positions)
    {

        const std::array<move::move_func, 4> directions{move::left_down, move::left_up,
                                                        move::right_down, move::right_up};
        piece.observed = 0;
        piece.attackable = 0;

        for (auto move_fn : directions)
        {
            auto current = piece.position;
            while (current != 0)
            {
                current = move_fn(current, board);
                piece.observed |= current;

                if ((current & positions.all_armies))
                {
                    if ((current & positions.hostile_armies))
                    {
                        // only pieces of enemies can be attacked
                        piece.attackable |= current;
                    }
                    break;
                } // squares in sight without a piece can be attacked
                piece.attackable |= current;
            }
        }
        piece.movable = piece.attackable;
    }
} // namespace