#include "piece-type.h"
#include <cstdlib>
#include <map>
#include <string>


namespace piece
{
    std::map<PieceType, std::string> lookup_string {
        // bin. search is slower then simply index, BUT std::cout not used in time critical context anyway
        {PieceType::KING, "King"},
        {PieceType::ROCK, "Rock"}
    };

    std::ostream &operator<<(std::ostream &out, const piece::PieceType& type) {
        out << lookup_string[type];
        return out;
    }
}
