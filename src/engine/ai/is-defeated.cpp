#include "chess-ai.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <vector>
#include "army.h"
#include "iterator-bitfield.h"
#include "piece-actions.h"
#include "piece.h"
#include "squares.h"
#include <cstdlib>
#include <numeric>
#include <map>
#include <climits>
#include <bit>
#include "notation.h"
#include "score.h"
#include "score-list.h"

namespace
{
    int get_number_of_living_pieces(const piece::army::Army &);
}

namespace ai {
    bool ChessAI::is_defeated(piece::army::Army &my_army) const
    {
        auto valid_moves = piece::api::calc_possible_moves(my_army, this->_board, this->_army_list);
        bool const are_there_valid_moves = valid_moves.size() > 0;
        bool const is_king_alive = my_army.king().is_alive();
        bool const have_multiple_pieces = get_number_of_living_pieces(my_army) > 1;
        return !(are_there_valid_moves & is_king_alive & have_multiple_pieces);
    }
}

namespace
{
    int get_number_of_living_pieces(const piece::army::Army &army)
    {
        int count = 0;
        for (auto piece : army.pieces)
        {
            if (piece.is_alive())
            {
                ++count;
            }
        }
        return count;
    }
} // namespace
