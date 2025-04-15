#pragma once
#include <cstdlib>
namespace board::bitmap {
    using Squares = u_int64_t;
}

namespace board {

    struct Board {
            u_int8_t num_of_squares_horizontal;
            u_int8_t num_of_squares_vertical;
            // u_int8_t valid_squares;
            Board(u_int8_t hor, u_int8_t ver)
                : num_of_squares_horizontal{hor},
                  num_of_squares_vertical{ver} {};
    };

}  // namespace board