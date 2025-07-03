#pragma once
#include <board.h>
#include <array>
#include <iterator>
#include "squares.h"

// TODO: refactor name bitfield <-> map + add namespace
class IteratorBitmap {
        board::bitmap::Squares _all;

    public:
        explicit IteratorBitmap(board::bitmap::Squares all) : _all(all) {}

        const board::bitmap::Squares operator*() const { return (_all & -_all); }

        IteratorBitmap& operator++() {
            _all = _all & ~(_all & -_all);
            return *this;
        }

        bool operator!=(const IteratorBitmap& other) const { return _all != other._all; }
};
