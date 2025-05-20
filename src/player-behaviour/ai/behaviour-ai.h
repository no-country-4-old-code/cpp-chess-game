#pragma once
#include "interface-player-behaviour.h"

class PlayerBehaviourAI : public IPlayerBehaviour
{
public:
    PlayerBehaviourAI(piece::army::army_list &list, const board::Board &board) : _army_list{list}, _board{board} {};
    void make_move(piece::army::Army &my_army) override;
    bool is_defeated(piece::army::Army &my_army) const override;

private:
    piece::army::army_list &_army_list;
    const board::Board &_board;
};
