#include "BuildingGrid.hpp"
#include "GridRenderer.hpp"
#include "MapEditor.hpp"
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
        aeris::BuildingGrid grid =
            aeris::createMapFromInput(std::cin, std::cout);

        std::cout << "\nYOUR BUILDING MAP\n";
        std::cout << "-----------------\n";

        aeris::renderGrid(grid, std::cout);

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
