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

    struct SimulationResult
    {
        ai::Move move;
        ai::score::ScoreList<SIZE> score;
    };

    struct ValueAndScore {
        ai::Move move{0, 0, {0, 0}};
        int value = std::numeric_limits<int>::min();
        ai::score::ScoreList<SIZE> score_list{};
    };

    const u_int8_t max_recursion = 9; // TODO: Depend on number of players

    ValueAndScore calc_value_for_move(const board::Board &board,const piece::army::army_list &army_list,
                            const size_t army_index, const u_int8_t recursions_count, const piece::api::ArmyDestinations& possible_moves) {
        ValueAndScore ret{};

        for (auto [src, destinations, extra] : possible_moves)
        {
            IteratorBitmap dest{destinations};

            while (*dest)
            {
                //
                auto notation = board::notation::ChessNotation(*dest, board);
                //
                auto copy_al = army_list;
                piece::api::move_piece(src, *dest, board, copy_al);
                if (extra.src != 0UL)
                {
                    // also execute extra action
                    piece::api::move_piece(extra.src, extra.dest, board, copy_al);
                }
                ai::score::ScoreList<SIZE> result = run_recursive_simulation(board, copy_al, (army_index + 1) % copy_al.size(), recursions_count + 1);

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


    ai::score::ScoreList<SIZE> run_recursive_simulation(const board::Board &board,
                                                        const piece::army::army_list &army_list,
                                                        const size_t army_index, const u_int8_t recursions_count)
    {

        if (army_list[army_index].pieces.size() == 0 || !army_list[army_index].king().is_alive())
        {
            // skip if army dead
            return run_recursive_simulation(board, army_list, (army_index + 1) % army_list.size(), recursions_count + 1);
        }

        if (recursions_count >= max_recursion)
        {
            return ai::score::score_list<SIZE>(board, army_list);
        }

        auto best_result = ai::score::score_list<SIZE>();
        ValueAndScore ret{};
        auto possible_moves = piece::api::calc_possible_moves(army_list[army_index], board, army_list);

        if (possible_moves.size() > 0)
        {
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
                    ai::score::ScoreList<SIZE> result = run_recursive_simulation(board, copy_al, (army_index + 1) % copy_al.size(), recursions_count + 1);
                    auto result_value = ai::score::calc_value_of_chess_position<SIZE>(result, army_index);

                    if (result_value > ret.value)
                    {
                        ret.score_list = result;
                        ret.value = result_value;
                        ret.move = {.src = src, .dest = *dest, .extra = extra};
                        if (ret.score_list[army_index].is_win())
                        {
                            return ret.score_list;
                        }
                    }

                    ++dest;
                }
            }
            return ret.score_list;
        }
        else
        {
            auto copy_al = army_list;
            const auto &king = army_list[army_index].king();
            bool is_king_under_attack = false;

            for (const auto &army : army_list)
            {
                if (army.color() != army_list[army_index].color())
                {
                    // enemy
                    for (const auto &enemy : army.pieces)
                    {
                        if (king.position & enemy.attackable)
                        {
                            is_king_under_attack = true;
                            if (is_king_under_attack)
                            {
                                break;
                            }
                        }
                    }
                }
            }

            if (!is_king_under_attack)
            {
                for (auto idx = 0; idx < copy_al.size(); ++idx)
                {
                    if (copy_al[idx].size() > 0 && copy_al[idx].king().is_alive())
                    {
                        // prefer fastest checkmate solution
                        best_result[idx] = ai::score::Score(ai::score::ranges::max_draw - recursions_count); // DRAW
                    }
                }
                return best_result;
            }

            copy_al[army_index].mark_as_defeated();

            int number_of_armies_alive = 0;
            for (auto idx = 0; idx < copy_al.size(); ++idx)
            {
                if (copy_al[idx].size() > 0 && copy_al[idx].king().is_alive())
                {
                    // prefer fastest checkmate solution
                    best_result[idx] = ai::score::Score(ai::score::ranges::max_win - recursions_count);
                    ++number_of_armies_alive;
                }
                else
                {
                    best_result[idx] = ai::score::Score();
                }
            }

            if (number_of_armies_alive == 1)
            {
                return best_result;
            }
            else
            {
                return run_recursive_simulation(board, copy_al, (army_index + 1) % army_list.size(), recursions_count + 1);
            }
        }

        return best_result;
    }

    
    SimulationResult run_simulation(const board::Board &board,
                                    const piece::army::army_list &army_list,
                                    const size_t army_index)
    {
        auto possible_moves = piece::api::calc_possible_moves(army_list[army_index], board, army_list);
        auto ret = calc_value_for_move(board,army_list,army_index, 0, possible_moves);
        return {ret.move, ret.score_list};
    }
}