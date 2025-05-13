#pragma once
#include <vector>
#include "interface-player-behaviour.h"
#include <aggregator-positions.h>

typedef int Bitmap;

class Piece {
    public:
        Bitmap attackable_fields;
        Bitmap movable_fields;
        bool color;
        int position;

        Piece();

        void set_position(int pos);
        void die();
        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;
};

class PlayerBehaviourAI : public IPlayerBehaviour {
    public:
        PlayerBehaviourAI(piece::aggregator::army_list& list, const board::Board& board): _army_list{list}, _board{board} {};
        void make_move(piece::army::Army& my_army) override;
        bool is_defeated(piece::army::Army &my_army) const override;

    private:
        // Generated from pseudo-code brainstorming. Corrected via AI
        piece::aggregator::army_list& _army_list;
        const board::Board& _board;

        bool myColor;
        static std::vector<Piece> get_all_pieces() ;
        static std::vector<Piece> filter(const std::vector<Piece>& pieces,
                                  bool color) ;
        static Bitmap create_attack_map(const std::vector<Piece>& pieces,
                                 bool color) ;
        static bool is_under_attack(const Piece& piece, Bitmap attackMap) ;
        static bool flip_a_coin() ;
        static Piece get_piece_on_dest(Bitmap dest) ;
        static Bitmap get_movable_fields_as_bitmap(bool color) ;
        static void evaluate_and_update_all_movable_fields(
            Piece& piece, const Piece& king,
            const std::vector<Piece>& pieces) ;
        static void update_movable_fields(bool color) ;
        static Piece get_king(const std::vector<Piece>& pieces) ;
        static Bitmap get_attacking_pieces(
            const Piece& king, const std::vector<Piece>& other_pieces) ;
        static void reset_movable_fields(std::vector<Piece>& pieces) ;
        static int count_bits(Bitmap bitmap) ;
};
