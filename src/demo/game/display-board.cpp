#include "display-board.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "notation.h"
#include "board-movements.h"
#include "color.h"
#include "pieces.h"
#include "piece-actions.h"
#include <bit>
#include <array>
#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include "piece-type.h"
#include <string_view>

using namespace board::notation::literal;

namespace
{
    // bin. search is slower then simply index, BUT std::cout not used in time critical context anyway

    std::map<piece::PieceType, char> lookup_piece_to_notation{
        {piece::PieceType::KING, 'K'},
        {piece::PieceType::ROCK, 'R'},
        {piece::PieceType::BISHOP, 'B'}};

    std::map<piece::PieceType, std::string_view> lookup_piece_to_name{
        {piece::PieceType::KING, "King"},
        {piece::PieceType::ROCK, "Rock"},
        {piece::PieceType::BISHOP, "Bishop"}};

    std::map<Color, char> lookup_color_to_notation{
        {Color::WHITE, 'w'},
        {Color::BLACK, 'b'},
        {Color::ORANGE, 'o'},
        {Color::BLUE, 'b'},
    };

    std::map<Color, std::string_view> lookup_color_to_name{
        {Color::WHITE, "White"},
        {Color::BLACK, "Black"},
        {Color::ORANGE, "Orange"},
        {Color::BLUE, "Blue"},
    };

    using TextPerSquare = std::vector<std::array<char, 2>>;
    TextPerSquare create_text_for_each_square(const board::Board &, const piece::army::army_list &);
    void print_text_for_each_square(const board::Board &, const TextPerSquare& );
}


namespace display
{
    void display_board(const board::Board &board, const piece::army::army_list &army_list)
    {
        auto squares = create_text_for_each_square(board, army_list);
        print_text_for_each_square(board, squares);
    }
}


std::ostream &operator<<(std::ostream &out, const Color color)
{

    if (lookup_color_to_name.contains(color)) {
        out << lookup_color_to_name[color];
    } else {
        out << "Unknown";
    }
    return out;
};

std::ostream &operator<<(std::ostream &out, const piece::PieceType &type)
{
    if (lookup_piece_to_name.contains(type)) {
        out << lookup_piece_to_name[type];
    } else {
        out << "Unknown";
    }
    return out;
}

namespace {

    TextPerSquare create_text_for_each_square(const board::Board &board, const piece::army::army_list &army_list) {
        const auto num_of_squares = board.num_of_squares_horizontal * board.num_of_squares_vertical;
        std::vector<std::array<char, 2>> squares;
        squares.resize(num_of_squares, {' ', ' '});

        for (auto army : army_list)
        {
            for (auto &piece : army.pieces)
            {
                if (piece.is_alive())
                {
                    assert(std::has_single_bit(piece.position));
                    const auto idx = std::countr_zero(piece.position);
                    squares[idx][0] = lookup_piece_to_notation[piece.type];
                    squares[idx][1] = lookup_color_to_notation[army.color()];
                }
            }
        }
        return squares;
    }

    void print_text_for_each_square(const board::Board &board, const TextPerSquare& squares) {
        const int bits_per_byte = 8;
        u_int row_count = 0;

        std::cout << "\nPrint Board\n\n";
        std::cout << " \t a   b   c   d   e   f   g   h \n";

        for (auto sqr : squares)
        {
            if (row_count % board.num_of_squares_horizontal == 0)
            {
                std::cout << "\n" << (row_count / bits_per_byte) + 1 << "\t";
            }
            std::cout << " " << sqr[0] << sqr[1] << " ";
            ++row_count;
        }
        std::cout << "\n\n" << std::flush;
    }
}