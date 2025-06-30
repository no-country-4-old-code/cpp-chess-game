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
#include "run-simulation.h"


ai::Move ChessAI::make_move(piece::army::Army &my_army)
{


    // find my index
    auto army_index = 0;
    for (auto army : this->_army_list)
    {
        if (army.color() == my_army.color())
        {
            break;
        }
        ++army_index;
    }

    auto result = ai::simulation::run_simulation(this->_board, this->_army_list, army_index);

    if (result.move.src)
    {
        piece::api::move_piece(result.move.src, result.move.dest, this->_board, this->_army_list);

        if (result.move.extra.src)
        {
            piece::api::move_piece(result.move.extra.src, result.move.extra.dest, this->_board, this->_army_list);
        }

        std::cout << "Moved from " << board::notation::ChessNotation(result.move.src, this->_board) << " to " << board::notation::ChessNotation(result.move.dest, this->_board) << std::endl;
    }
    else
    {
        my_army.mark_as_defeated();
        std::cout << "# CHECKMATE - No further move possible !!!\n"
                  << std::flush;
    }

    return result.move;
}
