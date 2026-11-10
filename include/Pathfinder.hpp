
#pragma once

#include "BuildingGrid.hpp"
#include "GridPosition.hpp"

#include <optional>
#include <vector>

namespace aeris {

using EvacuationPath = std::vector<GridPosition>;

// Finds a shortest path from the starting position
// to the nearest reachable exit using BFS.
//
// Returns std::nullopt if the start position is invalid,
// blocked, or no exit is reachable.
std::optional<EvacuationPath> findEvacuationPath(
    const BuildingGrid& grid,
    GridPosition start
);

}
