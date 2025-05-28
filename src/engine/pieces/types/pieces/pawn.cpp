#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"
#include <utility>
#include <map>

namespace {
    board::bitmap::Squares build_left_squares_mask(const board::Board &board);
    board::bitmap::Squares build_right_squares_mask(const board::Board &board);
    board::bitmap::Squares build_top_squares_mask(const board::Board &board);
    board::bitmap::Squares build_bottom_squares_mask(const board::Board &board);
}

namespace
{
    namespace move = board::movements;

    template <move::move_func MOVE, move::move_func ATTACK_1, move::move_func ATTACK_2>
    piece::Piece::update_fn_ptr build_update_fn()
    {
        return [] (piece::Piece &piece, const board::Board &board, const piece::Positions &positions) {
            /*
            if (piece.position == 0) {
                piece.type = PieceType::Queen;
                piece.update = queen::update;
                queen::update(piece, board, positions);
            } else {

            }
            */

            const auto attack_options = ATTACK_1(piece.position, board) | ATTACK_2(piece.position, board);
            const auto positions_my_army = positions.all_armies & ~positions.hostile_armies;
            auto move_option = MOVE(piece.position, board);  // Not movable IF blocked by enemy OR Friend

            if (!piece.has_moved())
            {
                if (move_option & ~positions.all_armies)
                {
                    // not blocked
                    move_option |= MOVE(move_option, board);
                }
            }

            piece.observed = attack_options | move_option;
            piece.attackable = attack_options & ~positions_my_army;
            piece.movable = move_option & ~positions.all_armies | piece.attackable & positions.hostile_armies;
        };

    }


    using builder_fn = piece::Piece::update_fn_ptr (*)();
    
    const std::map<piece::MoveDirection, builder_fn> lookup_builder{
        // movement options of pawn depend on generel direction.
        // e.g. in classic chess white pawns move DOWN and black pawns move UP
        {piece::MoveDirection::LEFT, build_update_fn<move::left, move::left_down, move::left_up>},
        {piece::MoveDirection::RIGHT, build_update_fn<move::right, move::right_down, move::right_up>},
        {piece::MoveDirection::UP, build_update_fn<move::up, move::left_up, move::right_up>},
        {piece::MoveDirection::DOWN, build_update_fn<move::down, move::left_down, move::right_down>}
    };


}

namespace piece
{

    Piece Pawn(board::Board board, board::notation::ChessNotation notation, MoveDirection direction)
    {
        return {PieceType::PAWN, notation.as_squares(board), lookup_builder.at(direction)()};
    }

}

namespace {
    board::bitmap::Squares build_left_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the left edge (a1, a2, a3,
        // ...)
        board::bitmap::Squares mask = 0;
        for (int i = 0; std::cmp_less(i, board.num_of_squares_vertical); i++) {
            mask |= 1ULL << (board.num_of_squares_horizontal * i);
        }
        return mask;
    }

    board::bitmap::Squares build_right_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the right edge (e.g. h1,
        // h2, h3, ...)
        board::bitmap::Squares mask                 = 0;
        board::bitmap::Squares const right_side_bit = 1ULL << (board.num_of_squares_horizontal - 1);
        for (int i = 0; std::cmp_less(i, board.num_of_squares_vertical); i++) {
            mask |= right_side_bit << (board.num_of_squares_horizontal * i);
        }
        return mask;
    }

    board::bitmap::Squares build_top_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the upper edge (a1, b1,
        // c1, ...)
        return (1ULL << board.num_of_squares_vertical) - 1;
    }

    board::bitmap::Squares build_bottom_squares_mask(const board::Board &board) {
        // Create a bitmap representing all square on the bottom edge (e.g. a8,
        // b8, c8, ...)
        return ~(~0ULL >> board.num_of_squares_vertical);
    }

}  // namespace