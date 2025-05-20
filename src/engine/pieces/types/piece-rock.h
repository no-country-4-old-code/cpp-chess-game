#pragma once
#include "piece-baseclass.h"
#include "piece-type.h"
#include "notation.h"

namespace piece::pieces::rock
{
    void update_observed_and_attackable(piece::Piece &, const board::Board &, sqrs, sqrs);
}

namespace piece::pieces
{
    class Rock : public Piece
    {
        /* Do not enter methods or data here.
           Only refer to stateless pure functions.
           Object slicing might otherwise cause side effect when used in local std::array.*/
    public:
        Rock(board::bitmap::Squares pos) : Piece(PieceType::ROCK, pos, rock::update_observed_and_attackable) {};
        Rock(board::Board board, board::notation::ChessNotation notation) : Piece(PieceType::ROCK, notation.as_squares(board), rock::update_observed_and_attackable) {};
    };
}
