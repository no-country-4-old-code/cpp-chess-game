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

namespace {
    int get_number_of_living_pieces(const piece::army::Army&);
}

using Score = StackVector<u_int16_t, piece::army::max_num_of_armies>;



struct SimulationResult {
    ai::Move move;
    Score score;
};

const u_int8_t max_recursion = 7;

std::map<piece::PieceType, u_int8_t> lookup_piece_value {
    {piece::PieceType::KING, 0},
    {piece::PieceType::QUEEN, 9},
    {piece::PieceType::ROCK, 4},
    {piece::PieceType::BISHOP, 4},
    {piece::PieceType::KNIGHT, 4},
    {piece::PieceType::PAWN, 1},
};


u_int16_t calc_piece_value(const piece::army::Army army) {
    auto current_value = 0;
    auto max_value = 0;

    for (const auto& piece: army.pieces) {
        max_value += lookup_piece_value[piece.type];
        if (piece.is_alive()) {
            current_value += lookup_piece_value[piece.type];
        }
    }

    return current_value * 100 / max_value; // norm to 0 - 100
}

u_int16_t calc_attack_value(const piece::army::Army& army, const board::Board& board) {
    board::bitmap::Squares current_value = 0;
    u_int16_t max_population_count = sizeof(board::bitmap::Squares) * CHAR_BIT;    

    for (const auto& piece: army.pieces) {
        current_value |= piece.attackable;
    }

    u_int16_t current_population = std::popcount(current_value);

    return current_population * 100 / max_population_count; // norm to 0 - 100
}



Score calc_score(const board::Board &board, const piece::army::army_list &army_list) {
    //std::cout << "Calc score " << std::endl;
    Score score;

    for (const auto& army: army_list) {
        if (army.size() > 0 && army.king().is_alive()) {
            score.push(calc_piece_value(army) + calc_attack_value(army, board));
        } else {
            score.push(0);
        }
    }
    return score;
}

Score create_empty_score(size_t size_army_list) {
    Score score;
    for (auto i = 0; i < size_army_list; ++i) {
        score.push(0);
    }
    return score;
}

int map_scores_to_value(Score scores, size_t current_army_index) {
    int sum = std::accumulate(scores.begin(), scores.end(), 0);
    return 2 * scores[current_army_index] - sum; 
}


Score run_recursive_simulation(const board::Board &board,
                                const piece::army::army_list &army_list,
                                const size_t army_index, const u_int8_t recursions_count) {
    
    if (army_list[army_index].pieces.size() == 0 || ! army_list[army_index].king().is_alive()) {
        // skip if army dead
        return run_recursive_simulation(board, army_list, (army_index + 1) % army_list.size(), recursions_count + 1);
    }

    if (recursions_count >= max_recursion) {
        return calc_score(board, army_list);
    }

    Score max_score = create_empty_score(army_list.size());
    int max_score_value = std::numeric_limits<int>::min();
    auto possible_moves = piece::api::calc_possible_moves(army_list[army_index], board, army_list);

    for (auto [src, destinations, extra] : possible_moves) {
        IteratorBitmap dest{destinations};
        
        while (*dest) {
            auto copy_al = army_list;
            piece::api::move_piece(src, *dest, board, copy_al);
            if (extra.src != 0UL) {
                // also execute extra action
                piece::api::move_piece(extra.src, extra.dest, board, copy_al);
            }
            Score result = run_recursive_simulation(board, copy_al, (army_index + 1) % copy_al.size(), recursions_count + 1);
            auto result_value = map_scores_to_value(result, army_index);

            if (result_value > max_score_value) {
                max_score = result;
                max_score_value = result_value;
            }

            ++dest;
        }

    }

    return max_score;
}


SimulationResult run_simulation(const board::Board &board,
                     const piece::army::army_list &army_list,
                     const size_t army_index) {

    u_int8_t recursions_count = 1;
    SimulationResult best_move = {
        .move = {0, 0, {0, 0}},
        .score = create_empty_score(army_list.size())
    };
    int max_score_value = std::numeric_limits<int>::min();

    auto possible_moves = piece::api::calc_possible_moves(army_list[army_index], board, army_list);

    for (auto [src, destinations, extra] : possible_moves) {
        IteratorBitmap dest{destinations};
        
        while (*dest) {
            auto copy_al = army_list;
            piece::api::move_piece(src, *dest, board, copy_al);
            if (extra.src != 0UL) {
                // also execute extra action
                piece::api::move_piece(extra.src, extra.dest, board, copy_al);
            }
            Score result = run_recursive_simulation(board, copy_al, (army_index + 1) % copy_al.size(), recursions_count + 1);

            auto result_value = map_scores_to_value(result, army_index);

            if (result_value > max_score_value) {
                max_score_value = result_value;
                best_move.score = result;
                best_move.move = {.src = src, .dest = *dest, .extra = extra };
            }

            ++dest;
        }
    }

    return best_move;
}



ai::Move ChessAI::make_move(piece::army::Army &my_army) {

    auto army_index = 0;
    for (auto army: this->_army_list) {
        if (army.color() == my_army.color()) {
            break;
        }
        ++army_index;
    }

    auto result = run_simulation(this->_board, this->_army_list, army_index);

    piece::api::move_piece(result.move.src, result.move.dest, this->_board, this->_army_list);
    std::cout << "Moved from " << board::notation::ChessNotation(result.move.src, this->_board) << " to " << 
    board::notation::ChessNotation(result.move.dest, this->_board)  << std::endl;

    if (result.move.extra.src) {
        piece::api::move_piece(result.move.extra.src, result.move.extra.dest, this->_board, this->_army_list);        
    }

    return result.move;
}

bool ChessAI::is_defeated(piece::army::Army &my_army) const {
    auto valid_moves = piece::api::calc_possible_moves(my_army, this->_board, this->_army_list);
    bool const are_there_valid_moves = valid_moves.size() > 0;
    bool const is_king_alive         = my_army.king().is_alive();
    bool const have_multiple_pieces  = get_number_of_living_pieces(my_army) > 1;
    return !(are_there_valid_moves & is_king_alive & have_multiple_pieces);
}

namespace {
    int get_number_of_living_pieces(const piece::army::Army &army) {
        int count = 0;
        for (auto piece : army.pieces) {
            if (piece.is_alive()) {
                ++count;
            }
        }
        return count;
    }
} // namespace
