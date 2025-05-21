#pragma once
#include "piece-baseclass.h"
#include "piece-type.h"
#include "notation.h"

namespace piece
{
    Piece King(board::Board board, board::notation::ChessNotation notation);
    Piece Rock(board::Board board, board::notation::ChessNotation notation);
    Piece Bishop(board::Board board, board::notation::ChessNotation notation);
}
