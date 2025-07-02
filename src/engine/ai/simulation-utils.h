#pragma once
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

namespace ai::simulation::utils
{
    inline bool should_be_skipped(const piece::army::Army& army) {

        return army.pieces.size() == 0 || army.is_defeated();
    }

    inline bool is_king_under_attack_fn(const piece::army::army_list &army_list, const size_t army_index) {
        const auto king_position = army_list[army_index].king().position;

        for (const auto &army : army_list)
            {
                if (army.color() != army_list[army_index].color())
                {
                    for (const auto &enemy : army.pieces)
                    {
                        if (king_position & enemy.attackable)
                        {
                            return true;
                        }
                    }
                }
            }
        return false;
    }
    
    inline bool is_more_then_one_army_alive(const piece::army::army_list& army_list) {
        u_int8_t count = 0;
        for (auto army: army_list) {
            count += static_cast<u_int8_t>(army.size() > 0 & army.is_defeated());
        }
        return count > 1;
    }

}