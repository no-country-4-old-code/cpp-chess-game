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

constexpr int SIZE = 4; // TODO: extract to template  & Should have same size as army_list

namespace ai::simulation
{

    ai::score::ScoreList<SIZE> run_recursive_simulation(const board::Board &board,
                                                        const piece::army::army_list &army_list,
                                                        const size_t army_index, const u_int8_t recursions_count);

    struct Result
    {
        ai::Move move{0, 0, {0, 0}};
        ai::score::ScoreList<SIZE> score_list{};
        int value = std::numeric_limits<int>::min();
    };

    const u_int8_t max_recursion = 9; // TODO: Depend on number of players

    Result find_best_move(const board::Board &board, const piece::army::army_list &army_list,
                          const size_t army_index, const u_int8_t recursions_count, const piece::api::ArmyDestinations &possible_moves)
    {
        Result ret{};

        for (auto [src, destinations, extra] : possible_moves)
        {
            IteratorBitmap dest{destinations};

            while (*dest)
            {
                auto copy_al = army_list;
                piece::api::move_piece(src, *dest, board, copy_al);
                if (extra.src != 0UL)
                {
                    // also execute extra action
                    piece::api::move_piece(extra.src, extra.dest, board, copy_al);
                }
                auto result = run_recursive_simulation(board, copy_al, (army_index + 1) % copy_al.size(), recursions_count + 1);
                auto result_value = ai::score::calc_value_of_chess_position<SIZE>(result, army_index);

                if (result_value > ret.value)
                {
                    ret.score_list = result;
                    ret.value = result_value;
                    ret.move = {.src = src, .dest = *dest, .extra = extra};
                }

                ++dest;
            }
        }
        return ret;
    }

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


    ai::score::ScoreList<SIZE> run_recursive_simulation(const board::Board &board,
                                                        const piece::army::army_list &army_list,
                                                        const size_t army_index, const u_int8_t recursions_count)
    {
        if (recursions_count >= max_recursion)
        {
            // Reaching a leaf - end recursive approach, calculate score and return it
            return ai::score::score_list<SIZE>(board, army_list);
        }

        if (should_be_skipped(army_list[army_index]))
        {
            // Skip, because a dead army does not move
            return run_recursive_simulation(board, army_list, (army_index + 1) % army_list.size(), recursions_count + 1);
        }

        const auto possible_moves = piece::api::calc_possible_moves(army_list[army_index], board, army_list);
        
        if (possible_moves.size() > 0)
        {
            // Going further along the tree and evaluate next moves
            auto ret = find_best_move(board, army_list, army_index, recursions_count, possible_moves);
            return ret.score_list;
        }

        if (!is_king_under_attack_fn(army_list, army_index))
        {
            // No moves left but king not under attack ? That´s a DRAW !
            return score::score_list_draw<SIZE>(army_list, recursions_count);
        }

        auto copy_al = army_list;
        copy_al[army_index].mark_as_defeated();

        if (is_more_then_one_army_alive(army_list)) {
            // One down - lets get the rest !
            return run_recursive_simulation(board, copy_al, (army_index + 1) % army_list.size(), recursions_count + 1);
        } else {
            // Path to victory was found. Return winning score-list
            return score::score_list_win<SIZE>(copy_al, recursions_count);
        }
    }


    Result run_simulation(const board::Board &board,
                          const piece::army::army_list &army_list,
                          const size_t army_index)
    {
        auto possible_moves = piece::api::calc_possible_moves(army_list[army_index], board, army_list);
        return find_best_move(board, army_list, army_index, 0, possible_moves);
    }
}