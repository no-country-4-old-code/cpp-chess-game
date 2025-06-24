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


namespace ai::score
{
    using uint = unsigned int;
    using ScoreList = StackVector<uint, piece::army::max_num_of_armies>;
    const uint min_score = 0;
    const uint max_score_regular = 1000;
    const uint score_draw = 2000;
    const uint score_win = 3000;

    ScoreList calc_score(const board::Board &board, const piece::army::army_list &army_list);
    ScoreList create_empty_score(size_t size_army_list);
    int map_scores_to_value(ScoreList scores, size_t current_army_index);
}