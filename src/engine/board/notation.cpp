#include "notation.h"
#include <sys/types.h>
#include <bit>
#include <cassert>
#include <cstddef>
#include <ostream>
#include <string_view>
#include "board.h"
#include "squares.h"

namespace {
    inline u_int8_t map_file_2_uint(char chr);
    inline u_int8_t map_rank_2_uint(char chr);
    inline char map_uint_2_file(u_int8_t offset);
    inline char map_uint_2_rank(u_int8_t offset);
    inline u_int8_t get_bit_index(const board::bitmap::Squares &squares);
}

namespace board::notation {

    ChessNotation::ChessNotation(std::string_view square_as_literal) {
        this->_file = square_as_literal[0];  // e.g. "a4" (file is "a") //NOLINT
        this->_rank = square_as_literal[1];  // e.g. "a4" (rank is "4") //NOLINT

        assert('a' <= this->_file && this->_file <= 'z');
        assert('1' <= this->_rank && this->_rank <= '9');
        assert(square_as_literal.size() == 2);  // e.g. "a4"
    };

    ChessNotation::ChessNotation(const board::bitmap::Squares &squares, const board::Board &board) {
        assert(std::has_single_bit(squares));

        const u_int8_t bit_index  = get_bit_index(squares);
        const u_int8_t horizontal = bit_index % board.num_of_squares_horizontal;
        const u_int8_t vertical   = bit_index / board.num_of_squares_horizontal;

        this->_file = map_uint_2_file(horizontal);
        this->_rank = map_uint_2_rank(vertical);
    };

    board::bitmap::Squares ChessNotation::as_squares(const board::Board &board) const {
        const u_int8_t horizontal = map_file_2_uint(this->_file);
        const u_int8_t vertical   = map_rank_2_uint(this->_rank);

        assert(horizontal < board.num_of_squares_horizontal);
        assert(vertical < board.num_of_squares_vertical);

        const u_int8_t bit_index = horizontal + (vertical * board.num_of_squares_horizontal);
        return 1ULL << bit_index;
    }

    std::ostream &operator<<(std::ostream &out, const board::notation::ChessNotation &notation) {
        out << notation._file << notation._rank;
        return out;
    };

}  // namespace board::notation

namespace board::notation::literal {
    ChessNotation operator""_n(const char *str, std::size_t len) {
        assert(len >= 2);
        std::string_view const view{str};
        return ChessNotation{view.substr(0, 2)};
    }
}

namespace {
    inline u_int8_t map_file_2_uint(const char chr) {
        return static_cast<u_int8_t>(chr - 'a');
    };

    inline u_int8_t map_rank_2_uint(const char chr) {
        return static_cast<u_int8_t>(chr - '1');
    };

    inline char map_uint_2_file(const u_int8_t offset) {
        const char chr = 'a' + offset;     // NOLINT
        assert('a' <= chr && chr <= 'z');  // catches narrowing conversion
        return chr;
    };

    inline char map_uint_2_rank(const u_int8_t offset) {
        const char chr = '1' + offset;     // NOLINT
        assert('1' <= chr && chr <= '9');  // catches narrowing conversion
        return chr;
    };

    inline u_int8_t get_bit_index(const board::bitmap::Squares &squares) {
        return static_cast<u_int8_t>(std::countr_zero(squares));
    };
}  // namespace
