#include "piece.h"
#include <cassert>
#include "board.h"
#include "piece-type.h"

namespace piece {

    void Piece::update_observed_and_attackable(const board::Board &board, sqrs pos_all,
                                               sqrs pos_hostile_armies) {
        assert(this->type != PieceType::_INVALID);
        update(*this, board, pos_all, pos_hostile_armies);
    };

    void Piece::mark_as_dead() {
        position   = 0;
        observed   = 0;
        attackable = 0;
    }

    bool Piece::is_alive() const {
        return position > 0;
    }

    bool Piece::operator==(const Piece &other) const {
        return position == other.position && observed == other.observed &&
               attackable == other.attackable && type == other.type;
    }

}  // namespace piece