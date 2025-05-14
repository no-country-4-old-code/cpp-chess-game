#include "piece-update.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "notation.h"
#include "board-movements.h"
#include "pieces-color.h"
#include "aggregator-positions.h"
#include "piece-king.h"
#include "piece-rock.h"
#include "piece-api.h"
#include <bit>
#include <array>
#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include "piece-type.h"

using namespace board::notation::literal;

namespace {

    const int bits_per_byte = 8;

    template <typename T, typename FUNCTION>
    void for_each_bit(T value, FUNCTION func) {
        // Iterate through every(!) bit starting with LSB
        u_int num_of_bits = bits_per_byte * sizeof(value);
        for(u_int i = 0; i < num_of_bits; ++i) {
            func(value & 1);
            value = value >> 1;
        }
    }

    std::map<piece::PieceType, char> lookup_piece_to_notation {
        // bin. search is slower then simply index, BUT std::cout not used in time critical context anyway
        {piece::PieceType::KING, 'K'},
        {piece::PieceType::ROCK, 'R'}
    };


    std::map<Color, char> lookup_color_to_notation {
        // bin. search is slower then simply index, BUT std::cout not used in time critical context anyway
        {Color::WHITE, '0'},
        {Color::BLACK, '1'},
        {Color::ORANGE, '2'},
        {Color::BLUE, '3'},
    };



}

namespace piece::update {

   

    void update_piece() {
        board::Board board{8, 8};

        using namespace piece;

        piece::aggregator::army_list army_list = {
            piece::army::Army{Color::BLUE, {
                pieces::King{"d3"_n.as_squares(board)},
                pieces::Rock{"e4"_n.as_squares(board)},
                pieces::Rock{"b4"_n.as_squares(board)},
                pieces::Rock{"b6"_n.as_squares(board)},                        
            }},
            piece::army::Army{Color::WHITE, {
                pieces::King{"d1"_n.as_squares(board)},
                pieces::Rock{"f8"_n.as_squares(board)},
                pieces::Rock{"g7"_n.as_squares(board)},
                pieces::Rock{"h6"_n.as_squares(board)},                        
            }},
            piece::army::Army{},
            piece::army::Army{}
        };

        
        auto my_piece = army_list[0].king();
        auto dest = "d4"_n.as_squares(board);
        piece::api::move_piece(my_piece.position, dest, board, army_list);

        // piece, board, aggr, hostile piece positions
        // function also there 
        // Call here
        // move_piece()
        // piece-update-position
        // piece-update-observed-attackable
        // piece-update-movable
        // piece-update-by-special-rules
        
        
        //piece::update::attackable::update_piece(my_piece, board, positions_all, positions_hostile_pieces);


        std::cout << "Other pieces\n";
        //display_bits_on_board(board, my_piece.attackable);
        std::cout << "Movements \n";
        //display_bits_on_board(board, my_piece.observed);

    }

    void display_all_pieces(const board::Board& board, const piece::aggregator::army_list& army_list) {
        std::cout << "\nPrint Board\n\n";
        std::cout << " \t a   b   c   d   e   f   g   h  \n";

        const auto num_of_squares = board.num_of_squares_horizontal * board.num_of_squares_vertical;
        std::vector<std::array<char, 2>> squares;
        squares.resize(num_of_squares, {' ', '\0'});

        for (auto& sqr: squares) {
            sqr[0] = ' ';
            sqr[1] = ' ';
        }

        for (auto army: army_list) {
            for (auto i=0; i < army.size(); ++i) {
                const piece::Piece* ptr = &army.pieces[i];
                if(ptr->is_alive()) {
                    assert(std::has_single_bit(ptr->position));
                    const auto idx = std::countr_zero(ptr->position);
                    squares[idx][0] = lookup_piece_to_notation[ptr->type]; 
                    squares[idx][1] = lookup_color_to_notation[army.color()];
                }
            }
        }

        u_int row_count = 1;
        std::cout << row_count << "\t";

        for (auto sqr: squares) {
            std::cout << " " << sqr[0] << sqr[1] << " ";
            if (row_count % board.num_of_squares_horizontal == 0) {
                std::cout << "\n" << (row_count / bits_per_byte) + 1 << "\t"; 
            }
            std::cout << std::flush;
            ++row_count;
        }

        std::cout << "\n";
    }

    
    void display_bits_on_board(const board::Board& board, board::bitmap::Squares sqrs) {
        std::cout << "\nPrint Board\n\n";
        std::cout << " \t a  b  c  d  e  f  g  h  \n";

        u_int bit_count = 1;
        std::cout << bit_count << "\t";

        for_each_bit(sqrs, [&bit_count, &board](bool is_bit_set) {
            if (is_bit_set) {
                std::cout << " X "; 
            } else {
                std::cout << " - "; 
            }

            if (bit_count % board.num_of_squares_horizontal == 0) {
                std::cout << "\n" << (bit_count / bits_per_byte) + 1 << "\t"; 
            }
            std::cout << std::flush;
            ++bit_count;
        });
        std::cout << "\n";
    }


}

