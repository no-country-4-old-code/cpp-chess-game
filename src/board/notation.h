#pragma once
#include <array>
#include <cstdlib>
#include <ostream>
#include "squares.h"

namespace board::notation {

    class ChessNotation {
        private:
            char _file;  // a, b, c, d, e ... (vertical line)
            char _rank;  // 1, 2, 3, 4, 5 ... (horizontal line)

        public:
            ChessNotation(const char square_as_literal[3]);  // e.g. "a1"
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
