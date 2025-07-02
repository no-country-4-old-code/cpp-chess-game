#include "chess-ai.h"
#include <cstdlib>
#include <ctime>
#include "army.h"
#include "piece-actions.h"
#include "piece.h"
#include <cstdlib>
#include <climits>

namespace
{

    bool is_only_king_alive(const piece::army::Army &army)
    {
        unsigned int count = 0;
        for (auto piece : army.pieces)
        {
            // predictable loop without if / else
            count += static_cast<unsigned int>(piece.is_alive());
        }
        return count < 2;
    }
}  // namespace

namespace ai {

    bool ChessAI::is_defeated(piece::army::Army &my_army) const
    {
        auto valid_moves = piece::api::calc_possible_moves(my_army, this->_board, this->_army_list);
        bool const no_valid_moves = valid_moves.size() == 0;
        bool const only_king_is_alive = is_only_king_alive(my_army);
        return no_valid_moves | my_army.is_defeated() | only_king_is_alive;
    }

}  // namespace ai
