
#pragma once

#include "OccupancyGrid.hpp"

#include <iosfwd>

namespace aeris {

void renderOccupancyGrid(
    const OccupancyGrid& grid,
    std::ostream& output
);

}
