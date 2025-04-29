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
#include <array>
#include <iostream>

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

    void display_board(const board::Board& board, board::bitmap::Squares sqrs) {
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
        piece::api::move_piece(my_piece, dest, board, army_list);

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
        display_bits_on_board(board, my_piece.attackable);
        std::cout << "Movements \n";
        display_bits_on_board(board, my_piece.observed);

    }

}

