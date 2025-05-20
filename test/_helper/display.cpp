#include "test-helper.h"
#include "piece-baseclass.h"
#include "board.h"
#include "squares.h"
#include "notation.h"
#include "board-movements.h"
#include "pieces-color.h"

#include "pieces.h"
#include "pieces.h"
#include "piece-actions.h"
#include <bit>
#include <array>
#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include "piece-type.h"

using namespace board::notation::literal;

namespace
{

    const int bits_per_byte = 8;

    template <typename T, typename FUNCTION>
    void for_each_bit(T value, FUNCTION func)
    {
        // Iterate through every(!) bit starting with LSB
        u_int num_of_bits = bits_per_byte * sizeof(value);
        for (u_int i = 0; i < num_of_bits; ++i)
        {
            func(value & 1);
            value = value >> 1;
        }
    }

}

namespace test::helper
{

    void display_bits_on_board(const board::Board &board, board::bitmap::Squares sqrs)
    {
        std::cout << "\nPrint Board\n\n";
        std::cout << " \t a  b  c  d  e  f  g  h  \n";

        u_int bit_count = 1;
        std::cout << bit_count << "\t";

        for_each_bit(sqrs, [&bit_count, &board](bool is_bit_set)
                     {
            if (is_bit_set) {
                std::cout << " X "; 
            } else {
                std::cout << " - "; 
            }

            if (bit_count % board.num_of_squares_horizontal == 0) {
                std::cout << "\n" << (bit_count / bits_per_byte) + 1 << "\t"; 
            }
            std::cout << std::flush;
            ++bit_count; });
        std::cout << "\n";
    }
}
