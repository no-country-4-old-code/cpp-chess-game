#include "board-movements.h"
#include "board.h"
#include "squares.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"

namespace
{
    void update(piece::Piece &piece, const board::Board &board, const piece::Positions &);
} 

namespace piece
{

    Piece Knight(board::Board board, board::notation::ChessNotation notation)
    {
        return {PieceType::KNIGHT, notation.as_squares(board), ::update};
    }
}

namespace
{
    namespace move = board::movements;

    template <move::move_func FN_DIRECTION_1, move::move_func FN_DIRECTION_2>
    board::bitmap::Squares knight_move(board::bitmap::Squares position, const board::Board &board)
    {
        auto tmp = FN_DIRECTION_1(position, board);
        tmp = FN_DIRECTION_2(tmp, board);
        return FN_DIRECTION_2(tmp, board);
    }


    void update(piece::Piece &piece, const board::Board &board, const piece::Positions &positions)
    {
        piece.observed = 0;
        piece.observed |= knight_move<move::left, move::up>(piece.position, board);
        piece.observed |= knight_move<move::left, move::down>(piece.position, board);
        piece.observed |= knight_move<move::right, move::up>(piece.position, board);
        piece.observed |= knight_move<move::right, move::down>(piece.position, board);
        piece.observed |= knight_move<move::up, move::left>(piece.position, board);
        piece.observed |= knight_move<move::up, move::right>(piece.position, board);
        piece.observed |= knight_move<move::down, move::left>(piece.position, board);
        piece.observed |= knight_move<move::down, move::right>(piece.position, board);

        auto positions_my_army = positions.all_armies & ~positions.hostile_armies;
        piece.attackable = piece.observed & ~positions_my_army;
        piece.movable = piece.attackable;
    }
} // namespace
