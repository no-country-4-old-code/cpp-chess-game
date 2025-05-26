#include "chess-ai.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <vector>
#include "army.h"
#include "iterator-bitfield.h"
#include "piece-actions.h"
#include "piece.h"
#include "squares.h"

namespace {
    int get_number_of_living_pieces(const piece::army::Army&);
}


void ChessAI::make_move(piece::army::Army &my_army) {
    struct Move {
            piece::Piece *piece;
            board::bitmap::Squares dest;
    };

    // === SELECTION
    auto moves_all = piece::api::calc_possible_moves(my_army, this->_board, this->_army_list);

    std::vector<Move> moves;
    std::vector<Move> attack;
    board::bitmap::Squares positions = 0;

    for (auto army : _army_list) {
        for (auto current : army.pieces) {
            positions |= current.position;
        }
    }

    for (auto [src, destinations] : moves_all) {
        IteratorBitmap dest{destinations};

        // just a temp. solution - iteration everytime is rather ugly
        piece::Piece *ptr = nullptr; //NOLINT: linter is wrong here
        for (auto &piece : my_army.pieces) {
            if (piece.position == src) {
                ptr = &piece;
                break;
            }
        }

        if (ptr != nullptr) {
            while ((*dest)) {
                if ((ptr->attackable & positions)) {
                    attack.emplace_back(ptr, *dest);
                } else {
                    moves.emplace_back(ptr, *dest);
                }
                ++dest;
            }
        }
    }

    std::cout << "- Found " << moves.size() << " possible moves for Player " << '\n';

    // MOVEMENT
    if (!attack.empty()) {
        auto [piece, dest] = attack[0];
        auto src           = piece->position;

        piece::api::move_piece(src, dest, this->_board, this->_army_list);
        // std::cout << "-> Attack with " << piece->type << " from square " <<
        // board::notation::ChessNotation{src, this->_board} << " to " <<
        // board::notation::ChessNotation{dest, this->_board} << std::endl;
    }

    else if (!moves.empty()) {
        auto idx           = static_cast<int>(rand() * moves.size()) % moves.size();
        auto [piece, dest] = moves[idx];
        auto src           = piece->position;

        piece::api::move_piece(src, dest, this->_board, this->_army_list);
        // std::cout << "-> Move " << piece->type << " from square " <<
        // board::notation::ChessNotation{src, this->_board} << " to " <<
        // board::notation::ChessNotation{dest, this->_board} << std::endl;
    } else {
        std::cout << "-> No moves left" << '\n';
    }
}

bool ChessAI::is_defeated(piece::army::Army &my_army) const {
    auto valid_moves = piece::api::calc_possible_moves(my_army, this->_board, this->_army_list);
    bool const are_there_valid_moves = valid_moves.size() > 0;
    bool const is_king_alive         = my_army.king().is_alive();
    bool const have_multiple_pieces  = get_number_of_living_pieces(my_army) > 1;
    return !(are_there_valid_moves & is_king_alive & have_multiple_pieces);
}

namespace {
    int get_number_of_living_pieces(const piece::army::Army &army) {
        int count = 0;
        for (auto piece : army.pieces) {
            if (piece.is_alive()) {
                ++count;
            }
        }
        return count;
    }
} // namespace
