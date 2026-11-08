
#include "GridRenderer.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASS] " << testName << '\n';
        ++passed;
    } else {
        std::cout << "[FAIL] " << testName << '\n';
        ++failed;
    }
}

int main() {

    // Test 1: Grid dimensions.
    aeris::BuildingGrid grid(5, 7);

    check(grid.rowCount() == 5, "Grid has 5 rows");
    check(grid.columnCount() == 7, "Grid has 7 columns");

    // Test 2: Default cell type.
    check(
        grid.at(0, 0).type == aeris::CellType::Wall,
        "New cells default to Wall"
    );

    // Test 3: Modify a cell.
    grid.at(2, 3).type = aeris::CellType::Door;

    check(
        grid.at(2, 3).type == aeris::CellType::Door,
        "Cell type can be modified"
    );

    // Test 4: Invalid row access.
    bool invalidRowRejected = false;

    try {
        (void)grid.at(5, 2);
    } catch (const std::out_of_range&) {
        invalidRowRejected = true;
    }

    check(invalidRowRejected, "Invalid row is rejected");

    // Test 5: Invalid column access.
    bool invalidColumnRejected = false;

    try {
        (void)grid.at(2, 7);
    } catch (const std::out_of_range&) {
        invalidColumnRejected = true;
    }

    check(invalidColumnRejected, "Invalid column is rejected");

    // Test 6: Zero-sized grid rejection.
    bool invalidDimensionsRejected = false;

    try {
        aeris::BuildingGrid invalidGrid(0, 7);
    } catch (const std::invalid_argument&) {
        invalidDimensionsRejected = true;
    }

    check(
        invalidDimensionsRejected,
        "Zero-sized grid is rejected"
    );

    // Test 7: Grid renderer.
    aeris::BuildingGrid visualGrid(6, 6);

    // Make the interior cells floors.
    for (std::size_t row = 1; row < 5; ++row) {
        for (std::size_t column = 1; column < 5; ++column) {
            visualGrid.at(row, column).type =
                aeris::CellType::Floor;
        }
    }

    // Add a door and an exit.
    visualGrid.at(2, 2).type = aeris::CellType::Door;
    visualGrid.at(4, 4).type = aeris::CellType::Exit;

    // Capture renderer output.
    std::ostringstream output;

    aeris::renderGrid(visualGrid, output);

    const std::string expected =
        "######\n"
        "#....#\n"
        "#.D..#\n"
        "#....#\n"
        "#...E#\n"
        "######\n";

    check(
        output.str() == expected,
        "Grid renderer displays the correct layout"
    );

    // Display test summary.
    std::cout << "\n========== TEST SUMMARY ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "==================================\n";

    return failed == 0 ? 0 : 1;
}
