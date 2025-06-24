#pragma once
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

using Score = StackVector<u_int16_t, piece::army::max_num_of_armies>;

namespace ai::score {
    u_int16_t calc_piece_value(const piece::army::Army army);
    u_int16_t calc_attack_value(const piece::army::Army& army, const board::Board& board);
    Score calc_score(const board::Board &board, const piece::army::army_list &army_list);
    Score create_empty_score(size_t size_army_list);
    int map_scores_to_value(Score scores, size_t current_army_index);
}