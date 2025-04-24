#pragma once
#include "pieces-color.h"
#include "piece.h"
#include "squares.h"
#include <cstdlib>
#include <vector>
#include <array>

namespace piece::army {

    const u_int8_t max_num_of_armies = 4; // equals max. number of player
    const u_int8_t max_pieces_per_army = 20; // equals max. pieces per player 

    class Army {
        private:
            Color _color;
            u_int8_t _size;
            u_int8_t _idx_king;
        public:
            std::array<piece::Piece, max_pieces_per_army> pieces;
            Army(): Army{Color::INVALID, std::vector<piece::Piece>{}} {};
            Army(Color color, const std::vector<piece::Piece>& pieces);
            Color color() const;
            u_int8_t size() const;
            piece::Piece king() const;
        };
}