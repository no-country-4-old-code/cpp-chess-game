#pragma once
#include "piece.h"
#include "board.h"

namespace piece::utils
{
    using sqrs = board::bitmap::Squares;
    sqrs create_embraced_squares_mask(sqrs, sqrs, const board::Board&);
}