#include "AgentManager.hpp"
#include "BuildingGrid.hpp"
#include "OccupancyGrid.hpp"
#include "SimulationRenderer.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

int main() {

    int passed = 0;
    int failed = 0;

    auto check = [&](bool condition, const std::string& name) {

        if (condition) {
            std::cout << "[PASS] " << name << '\n';
            ++passed;
        } else {
            std::cout << "[FAIL] " << name << '\n';
            ++failed;
        }
    };

    // Test 1: Render agents and original building symbols.
    aeris::BuildingGrid building(5, 7);

    for (std::size_t row = 1; row <= 3; ++row) {
        for (std::size_t column = 1; column <= 5; ++column) {
            building.at(row, column).type = aeris::CellType::Floor;
        }
    }

    building.at(1, 4).type = aeris::CellType::Door;
    building.at(3, 5).type = aeris::CellType::Exit;

    aeris::OccupancyGrid occupancy(building);
    aeris::AgentManager manager(occupancy);

    manager.addAgent({1, 1});
    const std::size_t trappedId = manager.addAgent({2, 3});
    const std::size_t evacuatedId = manager.addAgent({3, 5});

    manager.markTrapped(trappedId);
    manager.markEvacuated(evacuatedId);

    std::ostringstream output;

    aeris::renderSimulationGrid(building, manager, output);

    const std::string expected =
        "#######\n"
        "#1..D.#\n"
        "#..T..#\n"
        "#....E#\n"
        "#######\n";

    check(
        output.str() == expected,
        "Building grid displays agents, doors, exits and trapped agents"
    );

    // Test 2: Agent IDs above 9 use lowercase letters.
    aeris::BuildingGrid largerBuilding(3, 12);

    for (std::size_t column = 1; column <= 10; ++column) {
        largerBuilding.at(1, column).type = aeris::CellType::Floor;
    }

    aeris::OccupancyGrid largerOccupancy(largerBuilding);
    aeris::AgentManager largerManager(largerOccupancy);

    for (std::size_t column = 1; column <= 10; ++column) {
        largerManager.addAgent({1, column});
    }

    std::ostringstream largerOutput;

    aeris::renderSimulationGrid(
        largerBuilding,
        largerManager,
        largerOutput
    );

    const std::string largerExpected =
        "############\n"
        "#123456789a#\n"
        "############\n";

    check(
        largerOutput.str() == largerExpected,
        "Agent IDs above 9 display using lowercase letters"
    );

    std::cout << "\n========== SIMULATION RENDERER TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "================================================\n";

    return failed == 0 ? 0 : 1;
}
