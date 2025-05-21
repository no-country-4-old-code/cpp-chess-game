#include "army.h"
#include <sys/types.h>
#include <cassert>
#include <vector>
#include "color.h"
#include "piece-type.h"
#include "piece.h"

namespace piece::army {

    Army::Army(Color color, const std::vector<piece::Piece>& given_pieces)
        : _color{color} {
        assert(given_pieces.size() <= max_pieces_per_army);

        u_int8_t count_kings = 0;
        u_int8_t idx         = 0;
        _size                = static_cast<u_int8_t>(given_pieces.size());

        for (const auto& given : given_pieces) {
            pieces.push(given);

            if (given.type == piece::PieceType::KING) {
                _idx_king = idx;
                ++count_kings;
            }
            ++idx;
        }
        assert((count_kings == 1 ||
                given_pieces.empty()));  // Every army should have ONE king only
    }

    Color Army::color() const {
        return this->_color;
    }

    u_int8_t Army::size() const {
#ifndef NDEBUG
        // "size" is not updated after creation due performance reasons.
        // Adding / removing pieces is not allowed !
        if (_size == 0) {
            // empty army should stay empty
            assert(pieces[0].type == piece::PieceType::_INVALID);
        } else {
            // non-empty army should stay at same size
            assert(pieces[_size - 1].type != piece::PieceType::_INVALID);
            if (_size != max_pieces_per_army) {
                assert(pieces[_size].type == piece::PieceType::_INVALID);
            }
        }
#endif

        return this->_size;
    }

    const piece::Piece& Army::king() const {
        assert(this->pieces[this->_idx_king].type == piece::PieceType::KING);
        return (this->pieces[this->_idx_king]);
    }
}  // namespace piece::army