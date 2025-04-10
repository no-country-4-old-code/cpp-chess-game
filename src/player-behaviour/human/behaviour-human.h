#pragma once
#include "interface-player-behaviour.h"

class PlayerBehaviourHuman : public IPlayerBehaviour {
    public:
        void make_move() const override;
        bool has_valid_moves() const override;
};
