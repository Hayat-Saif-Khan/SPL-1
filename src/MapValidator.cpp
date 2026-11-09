
#include "MapValidator.hpp"

#include <queue>
#include <utility>

namespace aeris {

namespace {

// Determines whether a cell allows movement.
bool isWalkable(CellType type) {

    return type == CellType::Floor ||
           type == CellType::Door ||
           type == CellType::Exit;
}

}

// Validates a building using multi-source Breadth-First Search.
MapValidationResult validateMap(const BuildingGrid& grid) {

    MapValidationResult result;

    const std::size_t rows = grid.rowCount();
    const std::size_t columns = grid.columnCount();

    // Track which cells have been visited.
    std::vector<std::vector<bool>> visited(
        rows,
        std::vector<bool>(columns, false)
    );

    // BFS queue stores row-column coordinates.
    std::queue<std::pair<std::size_t, std::size_t>> frontier;

    // Find all evacuation exits.
    bool hasExit = false;

    for (std::size_t row = 0; row < rows; ++row) {

        for (std::size_t column = 0;
             column < columns;
             ++column) {

            if (grid.at(row, column).type == CellType::Exit) {

                hasExit = true;

                visited[row][column] = true;

                frontier.push({row, column});
            }
        }
    }

    // An evacuation map needs at least one exit.
    if (!hasExit) {
        result.errors.push_back(
            "The building must contain at least one exit."
        );
    }

    // Process all reachable cells using BFS.
    while (!frontier.empty()) {

        const auto [row, column] = frontier.front();

        frontier.pop();

        // Visits a neighbouring cell if it is walkable.
        const auto visitNeighbor =
            [&](std::size_t nextRow, std::size_t nextColumn) {

                if (!visited[nextRow][nextColumn] &&
                    isWalkable(grid.at(nextRow, nextColumn).type)) {

                    visited[nextRow][nextColumn] = true;

                    frontier.push({nextRow, nextColumn});
                }
            };

        // Up.
        if (row > 0) {
            visitNeighbor(row - 1, column);
        }

        // Down.
        if (row + 1 < rows) {
            visitNeighbor(row + 1, column);
        }

        // Left.
        if (column > 0) {
            visitNeighbor(row, column - 1);
        }

        // Right.
        if (column + 1 < columns) {
            visitNeighbor(row, column + 1);
        }
    }

    // Count walkable cells that cannot reach an exit.
    for (std::size_t row = 0; row < rows; ++row) {

        for (std::size_t column = 0;
             column < columns;
             ++column) {

            if (isWalkable(grid.at(row, column).type) &&
                !visited[row][column]) {

                ++result.unreachableCellCount;
            }
        }
    }

    // Report disconnected areas.
    if (result.unreachableCellCount > 0) {

        result.errors.push_back(
            std::to_string(result.unreachableCellCount) +
            " walkable cells cannot reach any exit."
        );
    }

    return result;
}

}
