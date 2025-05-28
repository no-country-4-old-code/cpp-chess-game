#include "board-movements.h"
#include "board.h"
#include "notation.h"
#include "piece-type.h"
#include "piece.h"
#include "pieces.h"
#include <map>

namespace
{
    namespace move = board::movements;

    template <move::move_func MOVE, move::move_func ATTACK_1, move::move_func ATTACK_2>
    void update(piece::Piece &piece, const board::Board &board, const piece::Positions &positions)
    {
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
    }

    const std::map<piece::MoveDirection, piece::Piece::update_fn_ptr> lookup_update_fn{
        // movement options of pawn depend on generel direction.
        // e.g. in classic chess white pawns move DOWN and black pawns move UP
        {piece::MoveDirection::LEFT, update<move::left, move::left_down, move::left_up>},
        {piece::MoveDirection::RIGHT, update<move::right, move::right_down, move::right_up>},
        {piece::MoveDirection::UP, update<move::up, move::left_up, move::right_up>},
        {piece::MoveDirection::DOWN, update<move::down, move::left_down, move::right_down>}
    };

}

namespace piece
{

    Piece Pawn(board::Board board, board::notation::ChessNotation notation, MoveDirection direction)
    {
        return {PieceType::PAWN, notation.as_squares(board), lookup_update_fn.at(direction)};
    }

}
