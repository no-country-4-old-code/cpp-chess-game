#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"
#include "update-linear-movement.h"

namespace piece
{
    Piece Queen(board::Board board, board::notation::ChessNotation notation)
    {
        namespace move = board::movements;
        return {
            PieceType::QUEEN,
            notation.as_squares(board),
            piece::movement::update_linear_movements<move::left_down, move::left_up, move::right_down, move::right_up,
                                                     move::left, move::right, move::up, move::down>
        };
    }
}  // namespace piece
