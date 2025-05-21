#pragma once
#include "piece-baseclass.h"
#include "piece-type.h"
#include "notation.h"

namespace piece
{

    Piece King(board::Board board, board::notation::ChessNotation notation);

    // ---

    namespace rock
    {
        void update_observed_and_attackable(piece::Piece &, const board::Board &, sqrs, sqrs);
    }

    class Rock : public Piece
    {
    public:
        Rock(board::Board board, board::notation::ChessNotation notation) : Piece(PieceType::ROCK, notation.as_squares(board), rock::update_observed_and_attackable) {};
    };

    // ---

    Piece Bishop(board::Board board, board::notation::ChessNotation notation);

    // ---

}
