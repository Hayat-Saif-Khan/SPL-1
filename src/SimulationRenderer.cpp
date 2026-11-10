#include "SimulationRenderer.hpp"

#include "AgentManager.hpp"
#include "BuildingGrid.hpp"

#include <ostream>

namespace aeris {

namespace {

// Converts a building cell type into its display symbol.
char symbolForCell(CellType type) {

    switch (type) {

        case CellType::Wall:
            return '#';

        case CellType::Floor:
            return '.';

        case CellType::Door:
            return 'D';

        case CellType::Exit:
            return 'E';
    }

    return '?';
}

// Converts an agent into a display symbol.
char symbolForAgent(const Agent& agent) {

    switch (agent.status) {

        case AgentStatus::Ready:
        case AgentStatus::Evacuating:

            if (agent.id >= 1 && agent.id <= 9) {
                return static_cast<char>('0' + agent.id);
            }

            if (agent.id >= 10 && agent.id <= 35) {
                return static_cast<char>('a' + agent.id - 10);
            }

            return '*';

        case AgentStatus::Trapped:
            return 'T';

        case AgentStatus::Evacuated:
            return '\0';
    }

    return '?';
}

}

// Displays the full building with agent positions.
void renderSimulationGrid(
    const BuildingGrid& grid,
    const AgentManager& manager,
    std::ostream& output
) {

    for (std::size_t row = 0;
         row < grid.rowCount();
         ++row) {

        for (std::size_t column = 0;
             column < grid.columnCount();
             ++column) {

            char symbol = symbolForCell(
                grid.at(row, column).type
            );

            // Overlay an active or trapped agent.
            for (const Agent& agent : manager.getAgents()) {

                if (agent.currentPosition.row == row &&
                    agent.currentPosition.column == column) {

                    char agentSymbol = symbolForAgent(agent);

                    // Evacuated agents do not cover the original cell.
                    if (agentSymbol != '\0') {
                        symbol = agentSymbol;
                        break;
                    }
                }
            }

            output << symbol;
        }

        output << '\n';
    }
}

}
