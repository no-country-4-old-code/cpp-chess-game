#include "army.h"
#include <sys/types.h>
#include <cassert>
#include <vector>
#include "color.h"
#include "piece-type.h"
#include "piece.h"

namespace piece::army
{

    Army::Army(Color color, const std::vector<piece::Piece> &given_pieces) : _color{color}
    {
        assert(given_pieces.size() <= max_pieces_per_army);
        u_int8_t count_kings = 0;
        u_int8_t idx = 0;

        for (const auto &given : given_pieces)
        {
            pieces.push(given);

            if (given.type == piece::PieceType::KING)
            {
                _idx_king = idx;
                ++count_kings;
            }
            ++idx;
        }
        assert((count_kings == 1 || given_pieces.empty())); // Every army should have ONE king only
    }

    void Army::mark_as_defeated()
    {
        for (auto &piece : this->pieces)
        {
            piece.mark_as_dead();
        }
    }

    Color Army::color() const
    {
        return this->_color;
    }

    u_int8_t Army::size() const
    {
        return this->pieces.size();
    }

    bool Army::is_defeated() const
    {
        // TODO: "army.pieces.size() == 0 " might be removed if template define army size !
        return this->size() == 0 || !this->king().is_alive();
    }

    const piece::Piece &Army::king() const
    {
        assert(this->pieces[this->_idx_king].type == piece::PieceType::KING);
        return (this->pieces[this->_idx_king]);
    }
} // namespace piece::army