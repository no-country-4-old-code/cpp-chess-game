#pragma once
#include "piece.h"
#include "piece-type.h"

namespace piece::pieces
{

    class Rock : public Piece
    {
    private:
    public:
        Rock(board::bitmap::Squares pos) : Piece(PieceType::ROCK, pos) {};
        void update_observed_and_attackable(const board::Board &board, sqrs pos_all, sqrs pos_hostile_armies) override;
    };

}
