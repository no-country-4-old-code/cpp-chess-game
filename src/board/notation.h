#pragma once
#include <cstdlib>
#include <ostream>
#include "squares.h"
#include <array>

namespace board::notation {

    class ChessNotation {
        private:
            char _file;  // a, b, c, d, e ... (vertical line)
            char _rank;  // 1, 2, 3, 4, 5 ... (horizontal line)

        public:
            ChessNotation(std::array<char, 2> square_as_literal);
            ChessNotation(const board::bitmap::Squares& squares,
                          const board::Board& board);
            board::bitmap::Squares as_squares(const board::Board& board) const;

            friend std::ostream& operator<<(
                std::ostream& out,
                const board::notation::ChessNotation& notation);
    };

    std::ostream& operator<<(std::ostream& out,
                             const board::notation::ChessNotation& notation);

}  // namespace board::notation
