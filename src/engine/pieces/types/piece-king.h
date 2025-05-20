#pragma once
#include "piece-baseclass.h"
#include "piece-type.h"
#include "notation.h"

namespace piece::pieces::king
{
    void update_observed_and_attackable(piece::Piece &, const board::Board &, sqrs, sqrs);
}

namespace piece::pieces
{
    class King : public Piece
    {
        /* Do not enter methods or data here.
           Only refer to stateless pure functions.
           Object slicing might otherwise cause side effect when used in local std::array.*/
    public:
        King(board::bitmap::Squares pos) : Piece(PieceType::KING, pos, king::update_observed_and_attackable) {};
        King(board::Board board, board::notation::ChessNotation notation) : Piece(PieceType::KING, notation.as_squares(board), king::update_observed_and_attackable) {};
    };
}
