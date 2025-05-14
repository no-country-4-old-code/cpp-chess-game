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

    /*

    void update_movable_pieces_of_army(piece::army::Army my_army, const board::Board &board, piece::aggregator::army_list &army_list)
    {
        board::bitmap::Squares enemy_attack_map = 0x12345;      // TODO all attackable from other armies
        board::bitmap::Squares enemy_observation_map = 0x12345; // TODO all observale from other armies
        board::bitmap::Squares positions_of_my_army = 0x123222; // TODO

        auto &king = my_army.king();

        if (king.position & enemy_attack_map)
        {
            int number_of_attackers = 1; // TODO

            //TODO: king.movable = king.attackable & ~enemy_observation_map;

            if (number_of_attackers == 1)
            {
                // only pieces which can intercept the threat to the king !
                piece::Piece *attacker = nullptr; //&my_army.pieces[1]; //  TODO: find attacker
                // get fields between attacker and king
                board::bitmap::Squares interceptable = 1234;
                for (auto& piece: my_army.pieces)
                {

                    if (piece.position == king.position)
                    {
                        continue;
                    }

                    //piece.movable = piece.attackable & interceptable;

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
            for (auto& piece:  my_army.pieces)
            {
                if (piece.position == king.position)
                {
                    // TODO: king.movable = king.attackable & ~enemy_observation_map;
                }
                else if (piece.position & enemy_attack_map)
                {
                    bool result = does_piece_movement_endanger_own_king(piece, board, army_list, my_army, positions_of_my_army, enemy_attack_map);

                    if (result)
                    {
                        // movement would lead to checkmate
                        //piece.movable = 0;
                    }
                    else
                    {
                        // free to move
                        //piece.movable = piece.observed;
                    }
                }
                else
                {
                    //piece.movable = piece.observed;
                }
            }
        }
    }
*/
}

namespace piece::api
{

    ArmyDestinations calc_possible_moves(const piece::army::Army& my_army, const board::Board&, const piece::aggregator::army_list&) {
        ArmyDestinations memory;
        for (size_t i=0; i < my_army.size(); ++i) {
            memory.push({my_army.pieces[i].position, my_army.pieces[i].observed});
        }
        return memory;
    }

    void move_piece(const board::bitmap::Squares src, const board::bitmap::Squares dest, const board::Board &board, piece::aggregator::army_list &army_list)
    {
        // remove old piece // TODO: there is one to remvoe
        for (auto &army : army_list)
        {
            for (auto& current: army.pieces)
            {
                if (current.position == src) {
                    current.position = dest;
                }

                else if (current.position == dest)
                {
                    current.mark_as_dead();
                }
            }
        }

        // aggregate positions // TODO: Optimize : Can be done together with the above
        piece::aggregator::PositionAggregator aggr{army_list};
        auto positions_all = aggr.positions();

        // update observed and attackable of all affected pieces
        auto affected_squares = src | dest;

        auto idx_army = 0;
        for (auto &army : army_list)
        {
            auto positions_hostile_pieces = positions_all & ~aggr.positions(idx_army);
            ++idx_army;
            for (auto& current: army.pieces)
            { 
                if ((current.observed | current.position) & affected_squares)
                {
                    current.update_observed_and_attackable(board, positions_all, positions_hostile_pieces);
                }
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

            for (auto& piece: army.pieces)
            {
                piece.update_observed_and_attackable(board, positions_all, positions_hostile_pieces);
            }
        }
    }
}
