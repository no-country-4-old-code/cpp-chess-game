#pragma once
#include <vector>
#include "interface-player-behaviour.h"

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
        PlayerBehaviourAI() : myColor{true} {};
        PlayerBehaviourAI(bool color);
        void make_move() const override;
        bool has_valid_moves() const override;

    private:
        // Generated from pseudo-code brainstorming. Corrected via AI
        bool myColor;
        std::vector<Piece> get_all_pieces() const;
        std::vector<Piece> filter(const std::vector<Piece>& pieces,
                                  bool color) const;
        Bitmap create_attack_map(const std::vector<Piece>& pieces,
                                 bool color) const;
        bool is_under_attack(const Piece& piece, Bitmap attackMap) const;
        bool flip_a_coin() const;
        Piece get_piece_on_dest(Bitmap dest) const;
        Bitmap get_movable_fields_as_bitmap(bool color) const;
        void evaluate_and_update_all_movable_fields(
            Piece& piece, const Piece& king,
            const std::vector<Piece>& pieces) const;
        void update_movable_fields(bool color) const;
        Piece get_king(const std::vector<Piece>& pieces) const;
        Bitmap get_attacking_pieces(
            const Piece& king, const std::vector<Piece>& other_pieces) const;
        void reset_movable_fields(std::vector<Piece>& pieces) const;
        int count_bits(Bitmap bitmap) const;
};
