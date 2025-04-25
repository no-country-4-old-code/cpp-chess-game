#pragma once
#include "piece.h"
#include "piece-type.h"

namespace piece::pieces {

    class King: public Piece {
        private:
        public:
            King(board::bitmap::Squares pos): Piece(PieceType::KING, pos) {};
            void update_observed_and_attackable(const board::Board& board, sqrs pos_all, sqrs pos_hostile_armies) override;
    };

}
