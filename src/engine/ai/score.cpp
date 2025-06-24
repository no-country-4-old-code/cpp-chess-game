#include "score.h"

namespace
{
    ai::score::Score calc_piece_value(const piece::army::Army &);
    ai::score::Score calc_attack_value(const piece::army::Army &, const board::Board &);
}

namespace ai::score
{

    ScoreList create_empty_score(size_t size_army_list)
    {
        ScoreList score;
        for (auto i = 0; i < size_army_list; ++i)
        {
            score.push(ranges::min);
        }
        return score;
    }

    ScoreList calc_score(const board::Board &board, const piece::army::army_list &army_list)
    {
        ScoreList score;

        for (const auto &army : army_list)
        {
            if (army.size() > 0 && army.king().is_alive())
            {
                score.push((calc_piece_value(army) + calc_attack_value(army, board)) / 2);
            }
            else
            {
                score.push(ranges::min);
            }
        }
        return score;
    }

    int map_scores_to_value(ScoreList scores, size_t current_army_index)
    {
        // value of an army depends on its score relative to scores of other armies
        int sum = std::accumulate(scores.begin(), scores.end(), 0);
        return 2 * scores[current_army_index] - sum;
    }
}

namespace
{

    std::map<piece::PieceType, u_int8_t> lookup_piece_value{
        {piece::PieceType::KING, 1}, // if king is checkmate all pieces are marked dead
        {piece::PieceType::QUEEN, 9},
        {piece::PieceType::ROCK, 4},
        {piece::PieceType::BISHOP, 4},
        {piece::PieceType::KNIGHT, 4},
        {piece::PieceType::PAWN, 1},
    };

    ai::score::Score calc_piece_value(const piece::army::Army &army)
    {
        ai::score::Score current_value = 0;
        ai::score::Score max_value = 0;

        for (const auto &piece : army.pieces)
        {
            max_value += lookup_piece_value[piece.type];
            if (piece.is_alive())
            {
                current_value += lookup_piece_value[piece.type];
            }
        }

        // return score normed to max_score
        return current_value * ai::score::ranges::max / max_value;
    }

    ai::score::Score calc_attack_value(const piece::army::Army &army, const board::Board &board)
    {
        board::bitmap::Squares current_value = 0;
        ai::score::Score max_population_count = sizeof(board::bitmap::Squares) * CHAR_BIT;

        for (const auto &piece : army.pieces)
        {
            current_value |= piece.attackable;
        }

        ai::score::Score current_population = std::popcount(current_value);
        // return score normed to max_score
        return current_population * ai::score::ranges::max / max_population_count;
    }

}
