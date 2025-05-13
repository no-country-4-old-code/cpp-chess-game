#pragma once
#include "army.h"

class IPlayerBehaviour
{
public:
    virtual ~IPlayerBehaviour() = 0;
    virtual void make_move(piece::army::Army &my_army) = 0;
    virtual bool is_defeated(piece::army::Army &my_army) const = 0;
};