#pragma once
#include "board.h"
#include "piece.h"
#include "squares.h"

#include "army.h"
#include "stack-vector.h"

namespace piece::api {

    struct Context
    {
        board::bitmap::Squares positions_all_armies;
        board::bitmap::Squares under_attack_map;
    };

    struct PieceDestinations {
            board::bitmap::Squares src{0};           // singular
            board::bitmap::Squares destinations{0};  // plural
            void (*run_additional_action)() = nullptr; // hook to e.g. move a second piece in case of castling
    };

    using ArmyDestinations = StackVector<PieceDestinations, piece::army::max_pieces_per_army>;

    ArmyDestinations calc_possible_moves(const piece::army::Army&, const board::Board&, const piece::army::army_list&);
    void init_army_list(piece::army::army_list&, const board::Board&);
    void move_piece(const board::bitmap::Squares, const board::bitmap::Squares, const board::Board&, piece::army::army_list&);
}

namespace piece::utils {
    using sqrs = board::bitmap::Squares;
    sqrs create_embraced_squares_mask(sqrs, sqrs, const board::Board &);
    void calc_special_moves (const piece::army::Army &my_army,
                                         const board::Board &board,
                                         const api::Context &context, api::ArmyDestinations& memory);
}