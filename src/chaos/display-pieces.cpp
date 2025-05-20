#include "display-pieces.h"
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

    std::map<piece::PieceType, char> lookup_piece_to_notation{
        // bin. search is slower then simply index, BUT std::cout not used in time critical context anyway
        {piece::PieceType::KING, 'K'},
        {piece::PieceType::ROCK, 'R'},
        {piece::PieceType::BISHOP, 'B'}};

    std::map<Color, char> lookup_color_to_notation{
        // bin. search is slower then simply index, BUT std::cout not used in time critical context anyway
        {Color::WHITE, 'w'},
        {Color::BLACK, 'b'},
        {Color::ORANGE, 'o'},
        {Color::BLUE, 'b'},
    };

}

namespace display
{

    void display_all_pieces(const board::Board &board, const piece::army::army_list &army_list)
    {
        std::cout << "\nPrint Board\n\n";
        std::cout << " \t a   b   c   d   e   f   g   h  \n";

        const auto num_of_squares = board.num_of_squares_horizontal * board.num_of_squares_vertical;
        std::vector<std::array<char, 2>> squares;
        squares.resize(num_of_squares, {' ', '\0'});

        for (auto &sqr : squares)
        {
            sqr[0] = ' ';
            sqr[1] = ' ';
        }

        for (auto army : army_list)
        {
            for (auto i = 0; i < army.size(); ++i)
            {
                const piece::Piece *ptr = &army.pieces[i];
                if (ptr->is_alive())
                {
                    assert(std::has_single_bit(ptr->position));
                    const auto idx = std::countr_zero(ptr->position);
                    squares[idx][0] = lookup_piece_to_notation[ptr->type];
                    squares[idx][1] = lookup_color_to_notation[army.color()];
                }
            }
        }

        u_int row_count = 1;
        std::cout << row_count << "\t";

        for (auto sqr : squares)
        {
            std::cout << " " << sqr[0] << sqr[1] << " ";
            if (row_count % board.num_of_squares_horizontal == 0)
            {
                std::cout << "\n"
                          << (row_count / bits_per_byte) + 1 << "\t";
            }
            std::cout << std::flush;
            ++row_count;
        }

        std::cout << "\n";
    }

}
