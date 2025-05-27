#include "piece.h"
#include <bit>
#include <cassert>
#include "board.h"
#include "piece-type.h"

namespace piece
{

    void Piece::update(const board::Board &board, sqrs pos_all, sqrs pos_hostile_armies)
    {
        assert(this->type != PieceType::_INVALID);
        update_fn(*this, board, Positions{.all_armies = pos_all, .hostile_armies = pos_hostile_armies});
    };

    void Piece::mark_as_dead()
    {
        position = 0;
        observed = 0;
        attackable = 0;
        movable = 0;
    }

    void Piece::move(board::bitmap::Squares dest)
    {
        assert(std::has_single_bit(dest));
        position = dest;
        observed = 0;
        attackable = 0;
        movable = 0;
        was_moved = true;
    }

    bool Piece::is_alive() const
    {
        return position > 0;
    }

    bool Piece::has_moved() const
    {
        return was_moved;
    }

    bool Piece::operator==(const Piece &other) const
    {
        return position == other.position && observed == other.observed &&
               attackable == other.attackable && type == other.type;
    }

} // namespace piece