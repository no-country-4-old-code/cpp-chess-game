#include "piece-api.h"
#include "piece.h"
#include "board.h"
#include "squares.h"
#include "aggregator-positions.h"
#include "piece-embraced-squares-mask.h"

namespace
{

    bool does_piece_movement_endanger_own_king(const piece::Piece &piece, const board::Board &board, const piece::aggregator::army_list &army_list, const piece::army::Army &my_army, const board::bitmap::Squares positions_of_my_army, const board::bitmap::Squares under_attack_map)
    {
        if (piece.position & under_attack_map) // only matter if piece is under attack at all
        {
            auto king_position = my_army.king().position;
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
                        if (king_position & enemy.attackable) {
                            // if king is already attacked by this piece, then movement of piece does not matter
                            continue;
                        }
                        // (optimize) add quick check if relative position to each other (if king left to piece then attacker has to be right to piece etc.)

                        // heavy load calcs:
                        auto tmp = enemy;
                        tmp.update_observed_and_attackable(board, positions_without_piece, positions_without_piece);

                        if (king_position & tmp.attackable )
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    

}

namespace piece::api
{

    ArmyDestinations calc_possible_moves(const piece::army::Army& my_army, const board::Board& board, const piece::aggregator::army_list& army_list) {
        
        board::bitmap::Squares enemy_attack_map = 0; 
        board::bitmap::Squares enemy_observation_map = 0;
        board::bitmap::Squares my_positions_map = 0;
        unsigned int number_of_king_attackers = 0;
        const piece::Piece* king_attacker = nullptr;
        const auto &king = my_army.king();
        ArmyDestinations memory;

        for (const auto& army: army_list) {
            if (army.color() == my_army.color()) {
                for (const auto& piece: army.pieces) {
                    my_positions_map |= piece.position;
                } 
            } else {
                for (const auto& piece: army.pieces) {
                    enemy_attack_map |= piece.attackable;
                    enemy_observation_map |= piece.observed;

                    if (piece.attackable & king.position) {
                        ++number_of_king_attackers;
                        king_attacker = &piece;
                    }
                }
            }
        }

        // king movement
        auto movable_king = king.attackable & ~enemy_observation_map;
        if (movable_king) {
            memory.push({king.position, movable_king});
        }

        // other pieces
        if (number_of_king_attackers == 0)
        {
            for (const auto& piece: my_army.pieces)
            {
                if (piece.position == king.position)
                {
                    continue;
                }
                else if (piece.position & enemy_attack_map)
                {
                    bool result = does_piece_movement_endanger_own_king(piece, board, army_list, my_army, my_positions_map, enemy_attack_map);

                    if (! result)
                    {
                         memory.push({piece.position, piece.attackable});
                    }
                }
                else
                {
                    memory.push({piece.position, piece.attackable});
                }
            }

        }
        else if (number_of_king_attackers == 1) {
            // get fields between attacker and king
            auto interceptable = utils::create_embraced_squares_mask(king_attacker->position, king.position, board);
            interceptable |= king_attacker->position;

            for (const auto& piece: my_army.pieces)
            {

                if (piece.position == king.position)
                {
                    continue;
                }

                auto movable = piece.attackable & interceptable;

                if (movable)
                {
                    bool result = does_piece_movement_endanger_own_king(piece, board, army_list, my_army, my_positions_map, enemy_attack_map);
                    if (! result)
                    {
                        memory.push({piece.position, movable});
                    }                    
                }
            }
        
        }
        return memory;
    }
    

    void move_piece(const board::bitmap::Squares src, const board::bitmap::Squares dest, const board::Board &board, piece::aggregator::army_list &army_list)
    {
        assert(std::has_single_bit(src));
        assert(std::has_single_bit(dest));
        assert(dest != src);

        std::array<board::bitmap::Squares, army::max_num_of_armies> army_positions_lookup;
        board::bitmap::Squares positions_all = 0;
        
        // update and calc positions
        for (auto i=0; i < army_list.size(); ++i)
        {
            army_positions_lookup[i] = 0;
            for (auto& current: army_list[i].pieces)
            {
                if (current.position == src) {
                    current.position = dest;
                }

                else if (current.position == dest)
                {
                    current.mark_as_dead();
                }

                army_positions_lookup[i] |= current.position;
                positions_all |= current.position;
            }
        }

        // update observed and attackable of all affected pieces
        auto affected_squares = src | dest;

        auto idx_army = 0;
        for (auto &army : army_list)
        {
            auto positions_hostile_pieces = positions_all & ~army_positions_lookup[idx_army];
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
        std::array<board::bitmap::Squares, army::max_num_of_armies> army_positions_lookup;
        board::bitmap::Squares positions_all = 0;

        // update and calc positions
        for (auto i=0; i < army_list.size(); ++i)
        {
            army_positions_lookup[i] = 0;
            for (auto& current: army_list[i].pieces)
            {
                army_positions_lookup[i] |= current.position;
                positions_all |= current.position;
            }
        }

        auto idx_army = 0;
        for (auto &army : army_list)
        {
            auto positions_hostile_pieces = positions_all & ~army_positions_lookup[idx_army];
            ++idx_army;

            for (auto& piece: army.pieces)
            {
                piece.update_observed_and_attackable(board, positions_all, positions_hostile_pieces);
            }
        }
    }
}
