#include "score.h"


namespace ai::score {
    std::map<piece::PieceType, u_int8_t> lookup_piece_value {
        {piece::PieceType::KING, 1}, // just placeholder - if king is checkmate all pieces are marked dead 
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
}