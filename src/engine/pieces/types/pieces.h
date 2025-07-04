#pragma once
#include "notation.h"
#include "piece-type.h"
#include "piece.h"

namespace piece {
    enum class MoveDirection { RIGHT, LEFT, UP, DOWN };

    Piece King(board::Board board, board::notation::ChessNotation notation);
    Piece Queen(board::Board board, board::notation::ChessNotation notation);
    Piece Rock(board::Board board, board::notation::ChessNotation notation);
    Piece Bishop(board::Board board, board::notation::ChessNotation notation);
    Piece Knight(board::Board board, board::notation::ChessNotation notation);
    Piece Pawn(board::Board board, board::notation::ChessNotation notation,
               MoveDirection direction);
}
