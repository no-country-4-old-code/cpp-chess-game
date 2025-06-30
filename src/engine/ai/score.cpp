#include "score.h"

namespace
{
    unsigned int calc_piece_value(const piece::army::Army &);
    unsigned int calc_attack_value(const piece::army::Army &, const board::Board &);
}

namespace ai::score
{

    Score::Score(const board::Board &board, const piece::army::Army &army)
    {
        if (army.size() > 0 && army.king().is_alive()) {
            this->_val = (calc_piece_value(army) + calc_attack_value(army, board)) / 2;
        } else {
            this->_val = ranges::min;
        }
    }

    bool Score::is_draw() const
    {
        return this->_val > ranges::max && this->_val < ranges::max_draw;
    }

    bool Score::is_win() const
    {
        return this->_val > ranges::max_draw;
    }

    unsigned int Score::value() const {
        return this->_val;
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

    unsigned int calc_piece_value(const piece::army::Army &army)
    {
        unsigned int current_value = 0;
        unsigned int max_value = 0;

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

    unsigned int calc_attack_value(const piece::army::Army &army, const board::Board &board)
    {
        board::bitmap::Squares current_value = 0;
        unsigned int max_population_count = sizeof(board::bitmap::Squares) * CHAR_BIT;

        for (const auto &piece : army.pieces)
        {
            current_value |= piece.attackable;
        }

        unsigned int current_population = std::popcount(current_value);
        // return score normed to max_score
        return current_population * ai::score::ranges::max / max_population_count;
    }

}
