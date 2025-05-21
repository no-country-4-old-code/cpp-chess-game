#pragma once
#include <array>
#include <cstdlib>
#include <vector>
#include "color.h"
#include "piece.h"
#include "squares.h"
#include "stack-vector.h"

namespace piece::army {
    const u_int8_t max_num_of_armies   = 4;   // equals max. number of player
    const u_int8_t max_pieces_per_army = 20;  // equals max. pieces per player

    class Army {
        private:
            Color _color;
            u_int8_t _idx_king;

        public:
            StackVector<piece::Piece, max_pieces_per_army> pieces;
            Army() : Army{Color::_INVALID, std::vector<piece::Piece>{}} {};
            Army(Color color, const std::vector<piece::Piece> &pieces);
            Color color() const;
            u_int8_t size() const;
            const piece::Piece &king() const;
    };

    using army_list = std::array<piece::army::Army, army::max_num_of_armies>;
}  // namespace piece::army