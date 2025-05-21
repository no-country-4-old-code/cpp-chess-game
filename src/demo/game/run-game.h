#pragma once
#include "color.h"
#include "player.h"
#include "chess-ai.h"
#include <board.h>
#include "display-pieces.h"
#include "pieces.h"
#include "notation.h"
#include "piece-actions.h"
#include <vector>

namespace demo::game{

    using PlayerGroup = std::vector<Player>;

    void run(PlayerGroup &group, const board::Board &board, piece::army::army_list &army_list);

}
