#pragma once
#include "piece-actions.h"

class ChessAI
{
public:
    ChessAI(piece::army::army_list &list, const board::Board &board) : _army_list{list}, _board{board} {};
    void make_move(piece::army::Army &my_army);
    bool is_defeated(piece::army::Army &my_army) const;

private:
    piece::army::army_list &_army_list;
    const board::Board &_board;
};
