
#pragma once

#include <cstddef>

namespace aeris {

struct GridPosition {

    std::size_t row{0};
    std::size_t column{0};

};

// Compare two grid positions.
inline bool operator==(
    const GridPosition& lhs,
    const GridPosition& rhs
) noexcept {

    return lhs.row == rhs.row &&
           lhs.column == rhs.column;
}

inline bool operator!=(
    const GridPosition& lhs,
    const GridPosition& rhs
) noexcept {

    return !(lhs == rhs);
}

}
