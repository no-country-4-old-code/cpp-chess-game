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

namespace ai::score::ranges
{
    // regular calculated scores are between "min" and "max".
    // scores with "max < score" but "score <= max_draw" indicate a "draw".
    // scores with "max_draw < score" indicate a "win".
    const unsigned int min = 0;
    const unsigned int max = 1000;
    const unsigned int max_draw = 2000;
    const unsigned int max_win = 3000;
}

namespace ai::score
{
    class Score
    {
    private:
        unsigned int _val{ranges::min};

    public:
        Score() = default;
        Score(unsigned int val): _val{val} {assert(_val <= ranges::max_win);};
        Score(const board::Board &, const piece::army::Army &);
        bool is_draw() const;
        bool is_win() const;
        unsigned int value() const;
    };

    template <unsigned int SIZE>
    using ScoreList = std::array<Score, SIZE>;

    template <unsigned int SIZE>
    ScoreList<SIZE> create_empty_score() {
        ScoreList<SIZE> score;
        for (auto &ele: score)
        {
            ele = Score();
        }
        return score;

        //return ScoreList<SIZE>{};
    };

    template <unsigned int SIZE>
    ScoreList<SIZE> fill_up_score_list(const board::Board& board, const piece::army::army_list& army_list)
    {
        ScoreList<SIZE> score;
        for (auto idx = 0; army_list.size() > idx; ++idx)
       {
            const auto& army = army_list[idx];
            if (army.size() > 0 && army.king().is_alive())
            {
                score[idx] = Score(board, army);
            }
            else
            {
                score[idx] = Score();
            }
        }
        return score;
    }

    template <unsigned int SIZE>
    int calc_value_of_chess_position(const ScoreList<SIZE>& scores, size_t current_army_index)
    {
        // value of an army depends on its score relative to scores of other armies
        unsigned sum = 0;
        for (const auto &score : scores)
        {
            sum += score.value();
        }

        return 2 * scores[current_army_index].value() - sum;
    }
}


