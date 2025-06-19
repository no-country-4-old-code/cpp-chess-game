#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"

namespace
{
    void update(piece::Piece &piece, const board::Board &board, const piece::Positions &positions)
    {
        const auto &position = piece.position;
        namespace move = board::movements;
        piece.attackable = move::left(position, board) | move::left_up(position, board) |
                         move::up(position, board) | move::right_up(position, board) |
                         move::right(position, board) | move::right_down(position, board) |
                         move::down(position, board) | move::left_down(position, board);

        auto positions_my_army = positions.all_armies & ~positions.hostile_armies;
        piece.movable = piece.attackable & ~positions_my_army;;      
        piece.observed = piece.attackable;
    }

} // namespace


namespace piece
{

    Piece King(board::Board board, board::notation::ChessNotation notation)
    {
        return {PieceType::KING, notation.as_squares(board), ::update};
    }

}
