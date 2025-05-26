#include <array>
#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"

namespace {
    void update_observed_and_attackable(piece::Piece&, const board::Board&, const piece::Positions&);
}

namespace piece {
    Piece Queen(board::Board board, board::notation::ChessNotation notation) {
        return {PieceType::QUEEN, notation.as_squares(board), ::update_observed_and_attackable};
    }
}

namespace {
    namespace move = board::movements;

    void update_observed_and_attackable(piece::Piece &piece, const board::Board &board, const piece::Positions& positions) {

        const std::array<move::move_func, 8> directions{move::left_down, move::left_up,
                                                        move::right_down, move::right_up,
                                                        move::left, move::right, move::up,
                                                        move::down};
        piece.observed   = 0;
        piece.attackable = 0;

        for (auto move_fn : directions) {
            auto current = piece.position;
            while (current != 0) {
                current = move_fn(current, board);
                piece.observed |= current;

                if ((current & positions.all_armies)) {
                    if ((current & positions.hostile_armies)) {
                        // only pieces of enemies can be attacked
                        piece.attackable |= current;
                    }
                    break;
                }  // squares in sight without a piece can be attacked
                piece.attackable |= current;
            }
        }
    }
}  // namespace