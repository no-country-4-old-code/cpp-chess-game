#pragma once
#include "piece.h"
#include "piece-type.h"

namespace piece::pieces::rock {
    void update_observed_and_attackable(piece::Piece&, const board::Board&, sqrs, sqrs);
}

namespace piece::pieces {
    class Rock: public Piece {
        /* Do not enter methods or data here.
           Only refer to stateless pure functions. 
           Object slicing might otherwise cause side effect when used in local std::array.*/
        public:
            Rock(board::bitmap::Squares pos): Piece(PieceType::ROCK, pos, rock::update_observed_and_attackable) {};
    };
}
