
#include "OccupancyRenderer.hpp"

#include <iostream>
#include <sstream>
#include <string>

int main() {

    int passed = 0;
    int failed = 0;

    // Create a building grid with 3 rows and 5 columns.
    aeris::BuildingGrid building(3, 5);

    // Configure traversable cells.
    building.at(1, 1).type = aeris::CellType::Floor;
    building.at(1, 2).type = aeris::CellType::Door;
    building.at(1, 3).type = aeris::CellType::Exit;

    // Generate occupancy grid.
    aeris::OccupancyGrid occupancy(building);

    // Capture the rendered output.
    std::ostringstream output;

    aeris::renderOccupancyGrid(occupancy, output);

    const std::string expected =
        "#####\n"
        "#...#\n"
        "#####\n";

    if (output.str() == expected) {
        std::cout << "[PASS] Occupancy grid displays correctly\n";
        ++passed;
    } else {
        std::cout << "[FAIL] Occupancy grid displays incorrectly\n";
        ++failed;
    }

    // Verify that door and exit are displayed as free cells.
    const std::string rendered = output.str();

    if (rendered.find("#...#") != std::string::npos) {
        std::cout << "[PASS] Floor, Door and Exit display as Free\n";
        ++passed;
    } else {
        std::cout << "[FAIL] Free cell representation is incorrect\n";
        ++failed;
    }

    std::cout << "\n========== OCCUPANCY RENDERER TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "==============================================\n";

    return failed == 0 ? 0 : 1;
}
