
#pragma once

#include "BuildingGrid.hpp"

#include <cstddef>

namespace aeris {

struct MapStatistics {

    std::size_t totalCells{0};

    std::size_t wallCount{0};
    std::size_t floorCount{0};
    std::size_t doorCount{0};
    std::size_t exitCount{0};

    std::size_t walkableCount{0};

    double blockedPercentage{0.0};
};

// Calculates statistics about a building map.
MapStatistics analyzeMap(const BuildingGrid& grid);

}
