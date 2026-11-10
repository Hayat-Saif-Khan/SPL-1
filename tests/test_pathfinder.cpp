
#include "Pathfinder.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& name) {

    if (condition) {
        std::cout << "[PASS] " << name << '\n';
        ++passed;
    } else {
        std::cout << "[FAIL] " << name << '\n';
        ++failed;
    }
}

// Creates a BuildingGrid from a text layout.
aeris::BuildingGrid makeGrid(
    const std::vector<std::string>& layout
) {

    const std::size_t rows = layout.size();
    const std::size_t columns = layout.front().size();

    aeris::BuildingGrid grid(rows, columns);

    for (std::size_t row = 0; row < rows; ++row) {

        if (layout[row].size() != columns) {
            throw std::invalid_argument("Inconsistent test layout.");
        }

        for (std::size_t column = 0; column < columns; ++column) {

            switch (layout[row][column]) {

                case '#':
                    grid.at(row, column).type = aeris::CellType::Wall;
                    break;

                case '.':
                    grid.at(row, column).type = aeris::CellType::Floor;
                    break;

                case 'D':
                    grid.at(row, column).type = aeris::CellType::Door;
                    break;

                case 'E':
                    grid.at(row, column).type = aeris::CellType::Exit;
                    break;

                default:
                    throw std::invalid_argument("Invalid test symbol.");
            }
        }
    }

    return grid;
}

// Checks that a path only uses traversable cells
// and moves one cell horizontally or vertically.
bool isValidPath(
    const aeris::BuildingGrid& grid,
    const aeris::EvacuationPath& path
) {

    if (path.empty()) {
        return false;
    }

    for (std::size_t i = 0; i < path.size(); ++i) {

        const auto position = path[i];

        if (position.row >= grid.rowCount() ||
            position.column >= grid.columnCount()) {
            return false;
        }

        const auto type = grid.at(
            position.row,
            position.column
        ).type;

        if (type == aeris::CellType::Wall) {
            return false;
        }

        if (i > 0) {

            const auto previous = path[i - 1];

            const std::size_t rowDifference =
                position.row > previous.row
                    ? position.row - previous.row
                    : previous.row - position.row;

            const std::size_t columnDifference =
                position.column > previous.column
                    ? position.column - previous.column
                    : previous.column - position.column;

            if (rowDifference + columnDifference != 1) {
                return false;
            }
        }
    }

    return true;
}

int main() {

    // Test 1: Basic pathfinding.
    auto grid = makeGrid({
        "#####",
        "#..E#",
        "#####"
    });

    auto path = aeris::findEvacuationPath(grid, {1, 1});

    check(
        path.has_value(),
        "BFS finds a reachable exit"
    );

    // Test 2: Path starts at the agent.
    check(
        path.has_value() &&
        path->front() == aeris::GridPosition{1, 1},
        "Path begins at the starting position"
    );

    // Test 3: Path ends at the exit.
    check(
        path.has_value() &&
        path->back() == aeris::GridPosition{1, 3},
        "Path ends at the evacuation exit"
    );

    // Test 4: Shortest path length.
    check(
        path.has_value() && path->size() == 3,
        "BFS calculates the shortest route"
    );

    // Test 5: Path validity.
    check(
        path.has_value() && isValidPath(grid, *path),
        "Path contains only valid adjacent cells"
    );

    // Test 6: Door traversal.
    auto doorGrid = makeGrid({
        "#####",
        "#.DE#",
        "#####"
    });

    auto doorPath =
        aeris::findEvacuationPath(doorGrid, {1, 1});

    check(
        doorPath.has_value() &&
        doorPath->size() == 3 &&
        doorGrid.at(
            doorPath->at(1).row,
            doorPath->at(1).column
        ).type == aeris::CellType::Door,
        "BFS can travel through doors"
    );

    // Test 7: Multiple exits.
    auto multipleExitGrid = makeGrid({
        "#######",
        "#..E..#",
        "#.....#",
        "#E....#",
        "#######"
    });

    auto multiplePath =
        aeris::findEvacuationPath(multipleExitGrid, {2, 3});

    check(
        multiplePath.has_value() &&
        multiplePath->back() == aeris::GridPosition{1, 3},
        "BFS selects the nearest reachable exit"
    );

    // Test 8: No reachable exit.
    auto disconnectedGrid = makeGrid({
        "########",
        "#..#...#",
        "#..#...#",
        "####E###",
        "########"
    });

    auto disconnectedPath =
        aeris::findEvacuationPath(disconnectedGrid, {1, 1});

    check(
        !disconnectedPath.has_value(),
        "BFS rejects unreachable exits"
    );

    // Test 9: Blocked starting position.
    auto blockedPath =
        aeris::findEvacuationPath(grid, {0, 0});

    check(
        !blockedPath.has_value(),
        "BFS rejects a starting position inside a wall"
    );

    // Test 10: Out-of-bounds starting position.
    auto invalidPath =
        aeris::findEvacuationPath(grid, {3, 1});

    check(
        !invalidPath.has_value(),
        "BFS rejects out-of-bounds starting positions"
    );

    // Test 11: Starting directly at an exit.
    auto exitGrid = makeGrid({
        "#####",
        "#.E.#",
        "#####"
    });

    auto exitPath =
        aeris::findEvacuationPath(exitGrid, {1, 2});

    check(
        exitPath.has_value() &&
        exitPath->size() == 1 &&
        exitPath->front() == aeris::GridPosition{1, 2},
        "Agent already at an exit needs no movement"
    );

    // Test 12: Route around a wall.
    auto obstacleGrid = makeGrid({
        "#######",
        "#..#..#",
        "#..#E.#",
        "#.....#",
        "#######"
    });

    auto obstaclePath =
        aeris::findEvacuationPath(obstacleGrid, {1, 1});

    check(
        obstaclePath.has_value() &&
        obstaclePath->size() == 7 &&
        isValidPath(obstacleGrid, *obstaclePath),
        "BFS finds a shortest route around obstacles"
    );

    std::cout << "\n========== PATHFINDER TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "======================================\n";

    return failed == 0 ? 0 : 1;
}
