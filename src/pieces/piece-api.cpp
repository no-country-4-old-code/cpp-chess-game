#include "piece-api.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "aggregator-positions.h"

namespace
{

    bool does_piece_movement_endanger_own_king(const piece::Piece &piece, const board::Board &board, const piece::aggregator::army_list &army_list, const piece::army::Army &my_army, board::bitmap::Squares positions_of_my_army, board::bitmap::Squares under_attack_map)
    {
        if (piece.position & under_attack_map)
        {
            auto king = my_army.king();
            auto positions_without_piece = positions_of_my_army & ~piece.position;
            // check enemy pieces
            for (auto &enemy_army : army_list)
            {
                if (enemy_army.color() == my_army.color())
                {
                    continue;
                }

                for (auto j = 0; j < enemy_army.size(); ++j)
                {
                    auto &enemy = enemy_army.pieces[j];

                    if (piece.position & enemy.attackable)
                    {
                        // (optimize) add quick check if relative position to each other (if king left to piece then attacker has to be right to piece etc.)

                        // heavy load calcs:
                        auto tmp = enemy;
                        tmp.update_observed_and_attackable(board, positions_without_piece, positions_without_piece);

                        if (king.position & tmp.attackable)
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    void reset_movable(piece::army::Army &army)
    {
        for (auto i = 0; i < army.size(); ++i)
        {
            army.pieces[i].movable = 0;
        }
    }

    void update_movable_pieces_of_army(piece::army::Army my_army, const board::Board &board, piece::aggregator::army_list &army_list)
    {
        board::bitmap::Squares enemy_attack_map = 0x12345;      // TODO all attackable from other armies
        board::bitmap::Squares enemy_observation_map = 0x12345; // TODO all observale from other armies
        board::bitmap::Squares positions_of_my_army = 0x123222; // TODO

        auto &king = my_army.king();
        reset_movable(my_army);

        if (king.position & enemy_attack_map)
        {
            int number_of_attackers = 1; // TODO

            king.movable = king.attackable & ~enemy_observation_map;

            if (number_of_attackers == 1)
            {
                // only pieces which can intercept the threat to the king !
                piece::Piece *attacker = &my_army.pieces[1]; //  TODO: find attacker
                // get fields between attacker and king
                board::bitmap::Squares interceptable = 1234;
                for (auto i = 0; i < my_army.size(); ++i)
                {
                    auto &piece = my_army.pieces[i];

                    if (piece.position == king.position)
                    {
                        continue;
                    }

                    piece.movable = piece.attackable & interceptable;

                    if (piece.movable)
                    {
                        bool result = does_piece_movement_endanger_own_king(piece, board, army_list, my_army, positions_of_my_army, enemy_attack_map);
                        if (result)
                        {
                            // movement would lead to checkmate
                            piece.movable = 0;
                        }
                    }
                }
            }
        }
        else
        {
            for (auto i = 0; i < my_army.size(); ++i)
            {
                auto &piece = my_army.pieces[i];

                if (piece.position == king.position)
                {
                    king.movable = king.attackable & ~enemy_observation_map;
                }
                else if (piece.position & enemy_attack_map)
                {
                    bool result = does_piece_movement_endanger_own_king(piece, board, army_list, my_army, positions_of_my_army, enemy_attack_map);

                    if (result)
                    {
                        // movement would lead to checkmate
                        piece.movable = 0;
                    }
                    else
                    {
                        // free to move
                        piece.movable = piece.observed;
                    }
                }
                else
                {
                    piece.movable = piece.observed;
                }
            }
        }
    }

}

namespace piece::api
{

    void move_piece(piece::Piece &piece, board::bitmap::Squares dest, const board::Board &board, piece::aggregator::army_list &army_list)
    {

        // set position of current piece
        auto old_position = piece.position;

        // remove old piece // TODO: there is one to remvoe
        for (auto &army : army_list)
        {
            for (auto i = 0; i < army.size(); ++i)
            {
                piece::Piece &current = army.pieces[i];

                if (current.position == dest)
                {
                    current.mark_as_dead();
                }
            }
        }

        piece.position = dest;

        // aggregate positions // TODO: Optimize : Can be done together with the above
        piece::aggregator::PositionAggregator aggr{army_list};
        auto positions_all = aggr.positions();

        // update observed and attackable of all affected pieces
        auto affected_squares = old_position | dest;

        auto idx_army = 0;
        for (auto &army : army_list)
        {
            auto positions_hostile_pieces = positions_all & ~aggr.positions(idx_army);
            ++idx_army;
            for (u_int8_t i = 0; i < army.size(); ++i)
            { // TODO: Overwrite iterator of army to use it like piece: army
                auto &current = army.pieces[i];

                if ((current.observed | current.position) & affected_squares)
                {
                    current.update_observed_and_attackable(board, positions_all, positions_hostile_pieces);
                }
            }
        }

        // update movable for all pieces
        for (auto &army : army_list)
        {
            for (auto i = 0; i < army.size(); ++i)
            {
                auto &current = army.pieces[i];
                // TODO: will be implemented later on
                current.movable = current.attackable;
            }
        }
    }

    void init_army_list(piece::aggregator::army_list &army_list, const board::Board &board)
    {

        piece::aggregator::PositionAggregator aggr{army_list};
        auto positions_all = aggr.positions();

        auto idx_army = 0;
        for (auto &army : army_list)
        {
            auto positions_hostile_pieces = positions_all & ~aggr.positions(idx_army);
            ++idx_army;

            for (u_int8_t i = 0; i < army.size(); ++i)
            {
                army.pieces[i].update_observed_and_attackable(board, positions_all, positions_hostile_pieces);
            }
        }

        // update movable for all pieces
        for (auto &army : army_list)
        {
            for (auto i = 0; i < army.size(); ++i)
            {
                auto &current = army.pieces[i];
                // TODO: will be implemented later on
                current.movable = current.attackable;
            }
        }
    }
}
