#pragma once
#include <score.h>

namespace ai::score
{
    template <unsigned int SIZE>
    using ScoreList = std::array<Score, SIZE>;

    template <unsigned int SIZE>
    ScoreList<SIZE> score_list()
    {
        return ScoreList<SIZE>{};
    };

    template <unsigned int SIZE>
    ScoreList<SIZE> score_list(const board::Board &board, const piece::army::army_list &army_list)
    {
        ScoreList<SIZE> score;
        assert(SIZE == army_list.size());
        for (auto idx = 0; army_list.size() > idx; ++idx)
        {
            score[idx] = Score(board, army_list[idx]);
        }
        return score;
    }

    template <unsigned int SIZE>
    ScoreList<SIZE> score_list_draw(const piece::army::army_list &army_list, int recursions_counter)
    {
        ScoreList<SIZE> score;
        assert(SIZE == army_list.size());
        for (auto idx = 0; army_list.size() > idx; ++idx)
        {   
            const auto& army = army_list[idx]; 
            if (army.size() > 0 && army.is_defeated()) {
                // should prefer DRAWs in less turns over DRAWs in more turns
                score[idx] = Score(ranges::max_draw - recursions_counter);
            } else {
                score[idx] = Score{};
            }
        }
        return score;
    }

    template <unsigned int SIZE>
    int calc_value_of_chess_position(const ScoreList<SIZE> &scores, size_t current_army_index)
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
