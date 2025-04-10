#pragma once
#include "interface-player-behaviour.h"
#include "pieces-color.h"

class Player {
    private:
        Color _color;
        IPlayerBehaviour* _behaviour;

    public:
        explicit Player(const Color color, IPlayerBehaviour* behaviour)
            : _color{color}, _behaviour{behaviour} {}
        [[nodiscard]] Color color() const;
        void make_move() const;
        bool has_valid_moves() const;
};

class PlayerBehaviourHuman : public IPlayerBehaviour {
    public:
        void make_move() const override;
        bool has_valid_moves() const override;
};

class PlayerBehaviourAI : public IPlayerBehaviour {
    public:
        void make_move() const override;
        bool has_valid_moves() const override;
};