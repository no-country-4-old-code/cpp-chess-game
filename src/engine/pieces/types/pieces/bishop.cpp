#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"
#include "update-linear-movement.h"

namespace piece
{
    Piece Bishop(board::Board board, board::notation::ChessNotation notation)
    {
        namespace move = board::movements;
        return {
            PieceType::BISHOP,
            notation.as_squares(board),
            piece::movement::update_linear_movements<move::left_down, move::left_up, move::right_down, move::right_up>
        };
    }
}  // namespace piece
