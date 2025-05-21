#include <array>
#include "board-movements.h"
#include "piece.h"
#include "pieces.h"

namespace {
    namespace move = board::movements;
    using namespace piece;

    void update_observed_and_attackable(piece::Piece &piece,
                                        const board::Board &board, sqrs pos_all,
                                        sqrs pos_hostile_armies) {
        const std::array<move::move_func, 4> directions{move::left, move::right,
                                                        move::up, move::down};

        piece.observed   = 0;
        piece.attackable = 0;

        for (auto go : directions) {
            auto current = piece.position;
            while (current != 0) {
                current = go(current, board);
                piece.observed |= current;

                if (current & pos_all) {
                    if (current & pos_hostile_armies) {
                        // only pieces of enemies can be attacked
                        piece.attackable |= current;
                    }
                    break;
                } else {
                    // squares in sight without a piece can be attacked
                    piece.attackable |= current;
                }
            }
        }
    }
}  // namespace

namespace piece {
    Piece Rock(board::Board board, board::notation::ChessNotation notation) {
        return Piece(PieceType::ROCK, notation.as_squares(board),
                     ::update_observed_and_attackable);
    }
}