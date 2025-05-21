#pragma once
#include "piece-baseclass.h"
#include "piece-type.h"
#include "notation.h"

namespace piece
{

    namespace king
    {
        void update_observed_and_attackable(piece::Piece &, const board::Board &, sqrs, sqrs);
    }

    class King : public Piece
    {
        /* Do not enter methods or data in this or following derived types.
           Only refer to stateless pure functions.
           Object slicing might otherwise cause side effect when used in local std::array.*/
    public:
        King(board::Board board, board::notation::ChessNotation notation) : Piece(PieceType::KING, notation.as_squares(board), king::update_observed_and_attackable) {};
    };



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
