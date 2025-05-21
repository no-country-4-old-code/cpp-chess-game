#pragma once
#include <board.h>
#include <vector>
#include "chess-ai.h"
#include "color.h"
#include "display-board.h"
#include "notation.h"
#include "piece-actions.h"
#include "pieces.h"
#include "player.h"

namespace demo::game {
    using PlayerGroup = std::vector<Player>;
    void run(PlayerGroup &group, const board::Board &board, piece::army::army_list &army_list);
}
