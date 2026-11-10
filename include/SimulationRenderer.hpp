#pragma once

#include <iosfwd>

namespace aeris {

class BuildingGrid;
class AgentManager;

// Displays the building grid with current agent positions.
void renderSimulationGrid(
    const BuildingGrid& grid,
    const AgentManager& manager,
    std::ostream& output
);

}
