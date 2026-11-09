
#include "OccupancyGrid.hpp"

#include <iostream>
#include <stdexcept>
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

    aeris::BuildingGrid building(3, 5);

    // Configure selected building cells.
    building.at(1, 1).type = aeris::CellType::Floor;
    building.at(1, 2).type = aeris::CellType::Door;
    building.at(1, 3).type = aeris::CellType::Exit;

    // Generate the occupancy grid.
    aeris::OccupancyGrid occupancy(building);

    // Test 1: Dimensions.
    check(
        occupancy.rowCount() == 3,
        "Occupancy grid has 3 rows"
    );

    check(
        occupancy.columnCount() == 5,
        "Occupancy grid has 5 columns"
    );

    // Test 2: Wall conversion.
    check(
        occupancy.at(0, 0) == aeris::OccupancyState::Blocked,
        "Wall becomes Blocked"
    );

    // Test 3: Floor conversion.
    check(
        occupancy.at(1, 1) == aeris::OccupancyState::Free,
        "Floor becomes Free"
    );

    // Test 4: Door conversion.
    check(
        occupancy.at(1, 2) == aeris::OccupancyState::Free,
        "Door becomes Free"
    );

    // Test 5: Exit conversion.
    check(
        occupancy.at(1, 3) == aeris::OccupancyState::Free,
        "Exit becomes Free"
    );

    // Test 6: Invalid row.
    bool invalidRowRejected = false;

    try {
        (void)occupancy.at(3, 0);
    } catch (const std::out_of_range&) {
        invalidRowRejected = true;
    }

    check(
        invalidRowRejected,
        "Invalid row is rejected"
    );

    // Test 7: Invalid column.
    bool invalidColumnRejected = false;

    try {
        (void)occupancy.at(0, 5);
    } catch (const std::out_of_range&) {
        invalidColumnRejected = true;
    }

    check(
        invalidColumnRejected,
        "Invalid column is rejected"
    );

    std::cout << "\n========== OCCUPANCY GRID TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "==========================================\n";

    return failed == 0 ? 0 : 1;
}
