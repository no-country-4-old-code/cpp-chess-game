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
        King(board::bitmap::Squares pos) : Piece(PieceType::KING, pos, king::update_observed_and_attackable) {};
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
        Rock(board::bitmap::Squares pos) : Piece(PieceType::ROCK, pos, rock::update_observed_and_attackable) {};
        Rock(board::Board board, board::notation::ChessNotation notation) : Piece(PieceType::ROCK, notation.as_squares(board), rock::update_observed_and_attackable) {};
    };

    // ---

    namespace bishop
    {
        void update_observed_and_attackable(piece::Piece &, const board::Board &, sqrs, sqrs);
    }

    class Bishop : public Piece
    {
    public:
        Bishop(board::Board board, board::notation::ChessNotation notation) : Piece(PieceType::BISHOP, notation.as_squares(board), piece::bishop::update_observed_and_attackable) {};
    };

    // ---

}
