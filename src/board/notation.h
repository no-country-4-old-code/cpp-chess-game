#pragma once
#include <array>
#include <cstdlib>
#include <ostream>
#include <string_view>
#include "board.h"
#include "squares.h"

namespace board::notation {

    class ChessNotation {
        private:
            char _file;  // a, b, c, d, e ... (vertical line)
            char _rank;  // 1, 2, 3, 4, 5 ... (horizontal line)

        public:
            ChessNotation(std::string_view square_as_literal);  // e.g. "a1"
            ChessNotation(const board::bitmap::Squares& squares,
                          const board::Board& board);
            auto operator<=>(const ChessNotation&) const = default;

            board::bitmap::Squares as_squares(const board::Board& board) const;

            friend std::ostream& operator<<(
                std::ostream& out,
                const board::notation::ChessNotation& notation);
    };

    std::ostream& operator<<(std::ostream& out,
                             const board::notation::ChessNotation& notation);
}  // namespace board::notation

namespace board::notation::literal {
    // own namespace for non-bloating "using namespace board::notation::literal"
    ChessNotation operator"" _n(const char* str, std::size_t len);
}