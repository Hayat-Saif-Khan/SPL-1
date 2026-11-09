#include "BuildingGrid.hpp"
#include "GridRenderer.hpp"
#include "MapEditor.hpp"
#include "MapValidator.hpp"
#include "MapAnalyzer.hpp"
#include "OccupancyGrid.hpp"
#include "OccupancyRenderer.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <exception>

// Displays the AERIS application banner.
void displayBanner() {
    std::cout << "\n========================================\n";
    std::cout << "                 AERIS\n";
    std::cout << " Adaptive Emergency & Incident Response\n";
    std::cout << "              Simulator\n";
    std::cout << "========================================\n";
}

// Displays the main application menu.
void displayMenu() {
    std::cout << "\nMAIN MENU\n";
    std::cout << "1. Building Map\n";
    std::cout << "2. Configure Simulation\n";
    std::cout << "3. Run Evacuation Simulation\n";
    std::cout << "4. Results and Analysis\n";
    std::cout << "5. Help / About\n";
    std::cout << "0. Exit\n";
    std::cout << "----------------------------------------\n";
}



void createManualBuildingMap() {

    try {

        // Step 1: Create the building map.
        aeris::BuildingGrid grid =
            aeris::createMapFromInput(std::cin, std::cout);

        // Step 2: Display the original building map.
        std::cout << "\nYOUR BUILDING MAP\n";
        std::cout << "-----------------\n";

        aeris::renderGrid(grid, std::cout);

        // Step 3: Validate the map using BFS.
        std::cout << "\n========== MAP VALIDATION ==========\n";

        const aeris::MapValidationResult result =
            aeris::validateMap(grid);

        // Step 4: Display validation results.
        if (result.isValid()) {

            std::cout << "\nMAP VALIDATION: PASSED\n";

            std::cout
                << "All walkable cells can reach at least one exit.\n";

        } else {

            std::cout << "\nMAP VALIDATION: FAILED\n";
            std::cout << "\nProblems found:\n";

            for (const auto& error : result.errors) {
                std::cout << "- " << error << '\n';
            }
        }

        // Step 5: Analyze the building map.
        const aeris::MapStatistics statistics =
            aeris::analyzeMap(grid);

        // Step 6: Display building statistics.
        std::cout << "\n========== BUILDING MAP ANALYSIS ==========\n";

        std::cout << "Grid Dimensions:       "
                  << grid.rowCount() << " x "
                  << grid.columnCount() << '\n';

        std::cout << "Total Cells:           "
                  << statistics.totalCells << '\n';

        std::cout << "\nCell Distribution:\n";

        std::cout << "Wall Cells:            "
                  << statistics.wallCount << '\n';

        std::cout << "Floor Cells:           "
                  << statistics.floorCount << '\n';

        std::cout << "Door Cells:            "
                  << statistics.doorCount << '\n';

        std::cout << "Exit Cells:            "
                  << statistics.exitCount << '\n';

        std::cout << "\nWalkable Cells:        "
                  << statistics.walkableCount << '\n';

        // Display blocked percentage with two decimal places.
        const auto oldFlags = std::cout.flags();
        const auto oldPrecision = std::cout.precision();

        std::cout << std::fixed << std::setprecision(2);

        std::cout << "Blocked Percentage:   "
                  << statistics.blockedPercentage << "%\n";

        // Restore previous output formatting.
        std::cout.flags(oldFlags);
        std::cout.precision(oldPrecision);

        std::cout << "\n============================================\n";

        // Step 7: Generate the occupancy grid.
        const aeris::OccupancyGrid occupancyGrid(grid);

        // Step 8: Display the occupancy grid.
        std::cout << "\n========== OCCUPANCY GRID ==========\n";

        aeris::renderOccupancyGrid(occupancyGrid, std::cout);

        std::cout << "\nLegend:\n";
        std::cout << "# = Blocked\n";
        std::cout << ". = Free\n";

        std::cout << "====================================\n";

    } catch (const std::exception& error) {

        std::cout << "\nMap creation stopped: "
                  << error.what() << '\n';
    }
}



int main() {
    int choice = -1;

    while (true) {
        displayBanner();
        displayMenu();

        std::cout << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "\nInvalid input. Please enter a number.\n";

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            continue;
        }

        switch (choice) {
            case 0:
                std::cout << "\nThank you for using AERIS. Goodbye!\n";
                return 0;

            case 1:
                 createManualBuildingMap();
                 break;

            case 2:
                std::cout << "\nSimulation Configuration is not implemented yet.\n";
                break;

            case 3:
                std::cout << "\nEvacuation Simulation is not implemented yet.\n";
                break;

            case 4:
                std::cout << "\nResults and Analysis are not implemented yet.\n";
                break;

            case 5:
                std::cout << "\nAERIS - Emergency Evacuation Simulator\n";
                std::cout << "SPL-1 Project, IIT, University of Dhaka\n";
                break;

            default:
                std::cout << "\nInvalid choice. Please select 0-5.\n";
        }

        std::cout << "\nPress Enter to return to the main menu...";

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cin.get();
    }

    return 0;
}
