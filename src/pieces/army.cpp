#include "army.h"
#include "piece-type.h"
#include <cassert>
#include <algorithm>

namespace piece::army
{

    Army::Army(Color color, const std::vector<piece::Piece> &given_pieces) : _color{color}
    {
        assert(given_pieces.size() <= max_pieces_per_army);

        u_int8_t count_kings = 0;
        _size = static_cast<u_int8_t>(given_pieces.size());

        for (u_int8_t i = 0; i < _size; ++i)
        {
            pieces[i] = given_pieces[i];

            if (pieces[i].type == piece::PieceType::KING)
            {
                _idx_king = i;
                ++count_kings;
            }
        }
        assert((count_kings == 1 || given_pieces.size() == 0)); // Every army should have ONE king only
    }

    Color Army::color() const
    {
        return this->_color;
    }

    u_int8_t Army::size() const
    {
#ifndef NDEBUG
        // size is not updated after creation due performance reasons
        // adding / removing pieces is not allowed !
        if (_size == 0)
        {
            // empty army should stay empty
            assert(pieces[0].type == piece::PieceType::_INVALID);
        }
        else
        {
            // non-empty army should stay at same size
            assert(pieces[_size - 1].type != piece::PieceType::_INVALID);
            if (_size != max_pieces_per_army)
            {
                assert(pieces[_size].type == piece::PieceType::_INVALID);
            }
        }
#endif

        return this->_size;
    }

    const piece::Piece& Army::king() const
    {
        assert(this->pieces[this->_idx_king].type == piece::PieceType::KING);
        return (this->pieces[this->_idx_king]);
    }
}