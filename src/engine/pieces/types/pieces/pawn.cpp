#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"
#include <map>
#include "board-edges.h"
#include "squares.h"
#include "update-linear-movement.h"

namespace {    
    namespace move = board::movements;
    using mask_build_fn = board::bitmap::Squares (*)(const board::Board&);

    inline void envolve_pawn_to_stronger_piece(piece::Piece&, const board::Board&, const piece::Positions&);

    template <move::move_func, move::move_func, move::move_func>
    inline void update_pawn(piece::Piece&, const board::Board&, const piece::Positions&);
    
    template <move::move_func, move::move_func, move::move_func, mask_build_fn>
    void update(piece::Piece&, const board::Board&, const piece::Positions&);

    const auto& right_edge_mask = board::edges::build_right_squares_mask;
    const auto& left_edge_mask = board::edges::build_left_squares_mask;
    const auto& top_edge_mask = board::edges::build_top_squares_mask;
    const auto& bottom_edge_mask = board::edges::build_bottom_squares_mask;

    const std::map<piece::MoveDirection, piece::Piece::update_fn_ptr> lookup_builder{
        // movement options of pawn depend on direction.
        // e.g. in classic chess white pawns move DOWN and black pawns move UP
        {piece::MoveDirection::LEFT, update<move::left, move::left_down, move::left_up, left_edge_mask>},
        {piece::MoveDirection::RIGHT, update<move::right, move::right_down, move::right_up, right_edge_mask>},
        {piece::MoveDirection::UP, update<move::up, move::left_up, move::right_up, top_edge_mask>},
        {piece::MoveDirection::DOWN, update<move::down, move::left_down, move::right_down, bottom_edge_mask>}
    };
}  // namespace


namespace piece
{

    Piece Pawn(board::Board board, board::notation::ChessNotation notation, MoveDirection direction)
    {
        return {PieceType::PAWN, notation.as_squares(board), lookup_builder.at(direction)};
    }

}

namespace {

    template <move::move_func Move, move::move_func Attack_1, move::move_func Attack_2, mask_build_fn Mask_edge>
    void update(piece::Piece &piece, const board::Board &board, const piece::Positions & positions)
    {
        if (piece.position & Mask_edge(board)) {
            // pawn reached end of board and envolve
            envolve_pawn_to_stronger_piece(piece, board, positions);
        } else {
            // run normal pawn update procedure
            update_pawn<Move, Attack_1, Attack_2>(piece, board, positions);
        }
    };


    inline void envolve_pawn_to_stronger_piece(piece::Piece &piece, const board::Board &board, const piece::Positions & positions) {
        piece.type = piece::PieceType::QUEEN;
        piece.update_fn = piece::movement::update_linear_movements<
                                                     move::left_down, move::left_up, move::right_down, move::right_up,
                                                     move::left, move::right, move::up, move::down>;
        piece.update(board, positions.all_armies , positions.hostile_armies);
    }


    template <move::move_func Move, move::move_func Attack_1, move::move_func Attack_2>
    inline void update_pawn(piece::Piece &piece, const board::Board &board, const piece::Positions & positions) {
        auto attack_options = Attack_1(piece.position, board) | Attack_2(piece.position, board);
        const auto positions_my_army = positions.all_armies & ~positions.hostile_armies;
        auto move_option = Move(piece.position, board);  // Not movable IF blocked by enemy OR Friend

        if (!piece.has_moved())
        {
            if (move_option & ~positions.all_armies)
            {
                move_option |= Move(move_option, board); // not blocked
            }
        }

        piece.observed = attack_options | move_option;
        piece.attackable = attack_options;
        piece.movable = move_option & ~positions.all_armies | piece.attackable & positions.hostile_armies;
    }

}  // namespace