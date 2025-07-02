#include "board-movements.h"
#include "board.h"
#include "piece.h"

namespace piece::movement {
    using move_func = board::movements::move_func;

    template <move_func Move>
    void move_until_blocked(piece::Piece &piece, const board::Board &board,
                            const piece::Positions &positions) {
        auto current = piece.position;
        auto blocking_positions =
            positions.all_armies & ~piece.position;  // remove own position from all
        while (current != 0 && ((current & blocking_positions) == 0)) {
            current = Move(current, board);
            piece.attackable |= current;
        }
    }

    template <move_func... Funcs>
    void update_linear_movements(piece::Piece &piece, const board::Board &board,
                                 const piece::Positions &positions) {
        piece.attackable = 0;
        (move_until_blocked<Funcs>(piece, board, positions), ...);
        auto positions_my_army = positions.all_armies & ~positions.hostile_armies;
        piece.observed         = piece.attackable;
        piece.movable          = piece.attackable & ~positions_my_army;
        ;
    }
}  // namespace piece::movement