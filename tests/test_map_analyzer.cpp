
#include "MapAnalyzer.hpp"

#include <cmath>
#include <iostream>
#include <string>

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

int main() {

    // Create a 4 x 5 building.
    aeris::BuildingGrid grid(4, 5);

    // Configure six walkable cells.
    grid.at(1, 1).type = aeris::CellType::Floor;
    grid.at(1, 2).type = aeris::CellType::Door;
    grid.at(1, 3).type = aeris::CellType::Exit;

    grid.at(2, 1).type = aeris::CellType::Floor;
    grid.at(2, 2).type = aeris::CellType::Floor;
    grid.at(2, 3).type = aeris::CellType::Door;

    const auto statistics = aeris::analyzeMap(grid);

    // Test 1: Total cells.
    check(
        statistics.totalCells == 20,
        "Total cells are counted correctly"
    );

    // Test 2: Walls.
    check(
        statistics.wallCount == 14,
        "Wall cells are counted correctly"
    );

    // Test 3: Floors.
    check(
        statistics.floorCount == 3,
        "Floor cells are counted correctly"
    );

    // Test 4: Doors.
    check(
        statistics.doorCount == 2,
        "Door cells are counted correctly"
    );

    // Test 5: Exits.
    check(
        statistics.exitCount == 1,
        "Exit cells are counted correctly"
    );

    // Test 6: Walkable cells.
    check(
        statistics.walkableCount == 6,
        "Walkable cells are counted correctly"
    );

    // Test 7: Blocked percentage.
    check(
        std::abs(statistics.blockedPercentage - 70.0) < 0.000001,
        "Blocked percentage is calculated correctly"
    );

    // Test 8: Category consistency.
    check(
        statistics.wallCount +
        statistics.floorCount +
        statistics.doorCount +
        statistics.exitCount == statistics.totalCells,
        "All cell categories sum to total cells"
    );

    // Test 9: A completely walkable building.
    aeris::BuildingGrid openGrid(2, 2);

    for (std::size_t row = 0; row < 2; ++row) {
        for (std::size_t column = 0; column < 2; ++column) {
            openGrid.at(row, column).type = aeris::CellType::Floor;
        }
    }

    const auto openStatistics = aeris::analyzeMap(openGrid);

    check(
        std::abs(openStatistics.blockedPercentage) < 0.000001,
        "Fully walkable map has zero blocked percentage"
    );

    // Test 10: A completely blocked building.
    aeris::BuildingGrid blockedGrid(2, 3);

    const auto blockedStatistics = aeris::analyzeMap(blockedGrid);

    check(
        std::abs(blockedStatistics.blockedPercentage - 100.0) < 0.000001,
        "Fully blocked map has 100 percent blocked cells"
    );

    std::cout << "\n========== MAP ANALYZER TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "========================================\n";

    return failed == 0 ? 0 : 1;
}
