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
    using Score = unsigned int;
    using ScoreList = StackVector<Score, piece::army::max_num_of_armies>;

    bool indicates_draw(Score);
    bool indicates_win(Score);
    ScoreList create_empty_score(size_t);
    ScoreList calc_score(const board::Board &, const piece::army::army_list &);
    int map_scores_to_value(ScoreList, size_t current_army_index);
}

namespace ai::score::ranges
{
    // regular calculated scores are between "min" and "max".
    // scores with "max < score" but "score <= max_draw" indicate a "draw".
    // scores with "max_draw < score" indicate a "win".
    const Score min = 0;
    const Score max = 1000;
    const Score max_draw = 2000;
    const Score max_win = 3000;
}