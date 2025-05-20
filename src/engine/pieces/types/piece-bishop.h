#pragma once
#include "piece.h"
#include "piece-type.h"
#include "notation.h"

namespace piece::pieces::bishop
{
    void update_observed_and_attackable(piece::Piece&, const board::Board&, sqrs, sqrs);
}


namespace piece::pieces {
    class Bishop: public Piece {
        /* Do not enter methods or data here.
           Only refer to stateless pure functions. 
           Object slicing might otherwise cause side effect when used in local std::array.*/
        public:
            Bishop(board::Board board, board::notation::ChessNotation notation): Piece(PieceType::BISHOP, notation.as_squares(board), piece::pieces::bishop::update_observed_and_attackable) {};

    };
}
