#pragma once

class IPlayerBehaviour {
    public:
        virtual ~IPlayerBehaviour()          = 0;
        virtual void make_move() const       = 0;
        virtual bool has_valid_moves() const = 0;
};