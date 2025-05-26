#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"



namespace {
    void update_observed_and_attackable(piece::Piece &piece, const board::Board &board,
                                        piece::sqrs pos_all, piece::sqrs pos_hostile_armies);
}  

namespace piece {

    Piece King(board::Board board, board::notation::ChessNotation notation) {
        return {PieceType::KING, notation.as_squares(board), ::update_observed_and_attackable};
    }

}

namespace {
    namespace move = board::movements;

    void update_observed_and_attackable(piece::Piece &piece, const board::Board &board,
                                        piece::sqrs pos_all, piece::sqrs pos_hostile_armies) {
        const auto &position = piece.position;
        piece.observed       = move::left(position, board) | move::left_up(position, board) |
                         move::up(position, board) | move::right_up(position, board) |
                         move::right(position, board) | move::right_down(position, board) |
                         move::down(position, board) | move::left_down(position, board);

        auto pos_own_pieces = pos_all & ~pos_hostile_armies;
        piece.attackable    = piece.observed & ~pos_own_pieces;  // can not attack own pieces
    }
}  // namespace
