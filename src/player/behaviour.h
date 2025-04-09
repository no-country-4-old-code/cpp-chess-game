#pragma once

class IPlayerBehaviour {
    public:
        virtual void make_move() const = 0;
        virtual ~IPlayerBehaviour()    = 0;
};

class PlayerBehaviourHuman : public IPlayerBehaviour {
    public:
        void make_move() const override;
};

class PlayerBehaviourAI : public IPlayerBehaviour {
    public:
        void make_move() const override;
};