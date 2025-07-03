#pragma once
#include <bit>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <numeric>
#include <ostream>
#include <vector>
#include "army.h"
#include "iterator-bitfield.h"
#include "piece-actions.h"
#include "piece.h"
#include "squares.h"

namespace ai::score::ranges {
    // regular calculated scores are between "min" and "max".
    // scores with "max < score" but "score <= max_draw" indicate a "draw".
    // scores with "max_draw < score" indicate a "win".
    const unsigned int min      = 0;
    const unsigned int max      = 1000;
    const unsigned int max_draw = 2000;
    const unsigned int max_win  = 3000;
}

namespace ai::score {
    class Score {
        private:
            unsigned int _val{ranges::min};

        public:
            Score() = default;
            Score(unsigned int val) : _val{val} { assert(_val <= ranges::max_win); };
            Score(const piece::army::Army &);
            bool is_draw() const;
            bool is_win() const;
            unsigned int value() const;
    };
}
