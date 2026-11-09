
#include "MapValidator.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int passed = 0;
int failed = 0;

// Reports the result of an individual test.
void check(bool condition, const std::string& name) {
    if (condition) {
        std::cout << "[PASS] " << name << '\n';
        ++passed;
    } else {
        std::cout << "[FAIL] " << name << '\n';
        ++failed;
    }
}

// Creates a BuildingGrid from a collection of map strings.
aeris::BuildingGrid makeGrid(const std::vector<std::string>& layout) {

    if (layout.empty() || layout.front().empty()) {
        throw std::invalid_argument("Test layout cannot be empty.");
    }

    const std::size_t rows = layout.size();
    const std::size_t columns = layout.front().size();

    aeris::BuildingGrid grid(rows, columns);

    for (std::size_t row = 0; row < rows; ++row) {

        if (layout[row].size() != columns) {
            throw std::invalid_argument("Inconsistent test row length.");
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

// Checks whether an error message contains a particular phrase.
bool containsError(
    const aeris::MapValidationResult& result,
    const std::string& phrase
) {
    return std::any_of(
        result.errors.begin(),
        result.errors.end(),
        [&](const std::string& error) {
            return error.find(phrase) != std::string::npos;
        }
    );
}

// Test 1: All walkable cells can reach an exit.
void testValidMap() {

    auto grid = makeGrid({
        "#######",
        "#.....#",
        "#..D.E#",
        "#.....#",
        "#######"
    });

    const auto result = aeris::validateMap(grid);

    check(result.isValid(),
          "Connected building map is valid");

    check(result.unreachableCellCount == 0,
          "Valid map has zero unreachable cells");
}

// Test 2: A map without an exit must be rejected.
void testMissingExit() {

    auto grid = makeGrid({
        "#####",
        "#...#",
        "#####"
    });

    const auto result = aeris::validateMap(grid);

    check(!result.isValid(),
          "Map without an exit is invalid");

    check(containsError(result, "at least one exit"),
          "Missing exit error is reported");

    check(result.unreachableCellCount == 3,
          "Walkable cells without an exit are counted");
}

// Test 3: A disconnected area must be detected.
void testDisconnectedArea() {

    auto grid = makeGrid({
        "########",
        "#..#...#",
        "#..#...#",
        "####E###",
        "########"
    });

    const auto result = aeris::validateMap(grid);

    check(!result.isValid(),
          "Disconnected building area is invalid");

    check(result.unreachableCellCount == 4,
          "Four unreachable cells are detected");
}

// Test 4: Doors must allow movement.
void testDoorConnectivity() {

    auto grid = makeGrid({
        "#####",
        "##D##",
        "##E##",
        "#####"
    });

    const auto result = aeris::validateMap(grid);

    check(result.isValid(),
          "Door is treated as a walkable cell");
}

// Test 5: Multiple exits must all be considered.
void testMultipleExits() {

    auto grid = makeGrid({
        "#########",
        "#E..#..E#",
        "#...#...#",
        "#########"
    });

    const auto result = aeris::validateMap(grid);

    check(result.isValid(),
          "Multiple disconnected regions with separate exits are valid");

    check(result.unreachableCellCount == 0,
          "All regions are reached by multi-source BFS");
}

// Test 6: Diagonal movement must not be allowed.
void testDiagonalConnectivity() {

    auto grid = makeGrid({
        "####",
        "#E##",
        "##.#",
        "####"
    });

    const auto result = aeris::validateMap(grid);

    check(!result.isValid(),
          "Diagonal-only connection is rejected");

    check(result.unreachableCellCount == 1,
          "Diagonal-only floor is detected as unreachable");
}

int main() {

    std::cout << "\n========== MAP VALIDATOR TESTS ==========\n\n";

    testValidMap();
    testMissingExit();
    testDisconnectedArea();
    testDoorConnectivity();
    testMultipleExits();
    testDiagonalConnectivity();

    std::cout << "\n========== TEST SUMMARY ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "==================================\n";

    return failed == 0 ? 0 : 1;
}
