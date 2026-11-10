
#include "Pathfinder.hpp"

#include <algorithm>
#include <optional>
#include <queue>
#include <vector>

namespace aeris {

namespace {

// Determines whether a building cell allows movement.
bool isWalkable(CellType type) {

    return type == CellType::Floor ||
           type == CellType::Door ||
           type == CellType::Exit;
}

// Reconstructs a path using the parent coordinates.
EvacuationPath reconstructPath(
    GridPosition start,
    GridPosition destination,
    const std::vector<
        std::vector<std::optional<GridPosition>>
    >& parents
) {

    EvacuationPath path;

    GridPosition current = destination;

    while (current != start) {

        path.push_back(current);

        current = *parents[current.row][current.column];
    }

    // Include the starting position.
    path.push_back(start);

    // Reverse so the path goes from start to exit.
    std::reverse(path.begin(), path.end());

    return path;
}

}

// Finds the shortest route from a starting position to an exit.
std::optional<EvacuationPath> findEvacuationPath(
    const BuildingGrid& grid,
    GridPosition start
) {

    const std::size_t rows = grid.rowCount();
    const std::size_t columns = grid.columnCount();

    // Check whether the starting position is inside the grid.
    if (start.row >= rows || start.column >= columns) {
        return std::nullopt;
    }

    // An agent cannot start inside a wall.
    if (!isWalkable(grid.at(start.row, start.column).type)) {
        return std::nullopt;
    }

    // Tracks which cells have already been explored.
    std::vector<std::vector<bool>> visited(
        rows,
        std::vector<bool>(columns, false)
    );

    // Stores the previous position for each visited cell.
    std::vector<
        std::vector<std::optional<GridPosition>>
    > parents(
        rows,
        std::vector<std::optional<GridPosition>>(columns)
    );

    // BFS queue.
    std::queue<GridPosition> frontier;

    // Initialize the search.
    frontier.push(start);
    visited[start.row][start.column] = true;

    while (!frontier.empty()) {

        // Get the next position.
        const GridPosition current = frontier.front();

        frontier.pop();

        // Check whether an exit has been reached.
        if (grid.at(current.row, current.column).type ==
            CellType::Exit) {

            return reconstructPath(start, current, parents);
        }

        // Explore neighboring cells.
        const auto visitNeighbor =
            [&](std::size_t nextRow, std::size_t nextColumn) {

                if (!visited[nextRow][nextColumn] &&
                    isWalkable(
                        grid.at(nextRow, nextColumn).type
                    )) {

                    visited[nextRow][nextColumn] = true;

                    // Remember where this cell came from.
                    parents[nextRow][nextColumn] = current;

                    frontier.push({nextRow, nextColumn});
                }
            };

        // UP.
        if (current.row > 0) {
            visitNeighbor(current.row - 1, current.column);
        }

        // Down.
        if (current.row + 1 < rows) {
            visitNeighbor(current.row + 1, current.column);
        }

        // Left.
        if (current.column > 0) {
            visitNeighbor(current.row, current.column - 1);
        }

        // Right.
        if (current.column + 1 < columns) {
            visitNeighbor(current.row, current.column + 1);
        }
    }

    // No reachable exit was found.
    return std::nullopt;
}

}
