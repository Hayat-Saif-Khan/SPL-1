#include "BuildingGrid.hpp"
#include "GridRenderer.hpp"
#include "MapEditor.hpp"
#include "MapValidator.hpp"
#include "MapAnalyzer.hpp"
#include "OccupancyGrid.hpp"
#include "OccupancyRenderer.hpp"
#include "AgentManager.hpp"
#include "EvacuationSimulation.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <exception>
#include <memory>
#include <optional>
#include <utility>


struct AppSession {

    // Stores the validated building map.
    std::optional<aeris::BuildingGrid> building;

    // Manages evacuation agents.
    std::unique_ptr<aeris::AgentManager> agents;

    // Manages the running simulation.
    std::unique_ptr<aeris::EvacuationSimulation> simulation;

    // Stores additional simulation conditions.
    bool simulationStalled{false};
    bool simulationStepLimitReached{false};
};

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



void createManualBuildingMap(AppSession& session) {
    // Clear the previous simulation and agents.
    session.simulation.reset();
    session.agents.reset();
    session.building.reset();

    session.simulationStalled = false;
    session.simulationStepLimitReached = false;

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

        // Save the building only when validation succeeds.
    if (result.isValid()) {

        session.building = grid;

        std::cout << "\nBuilding map saved successfully.\n";

    } else {

        std::cout << "\nInvalid building map was not saved.\n";
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




bool readInteger(const std::string& prompt, int& value) {

    while (true) {

        std::cout << prompt;

        if (std::cin >> value) {
            return true;
        }

        if (std::cin.eof()) {
            return false;
        }

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Invalid input. Please enter an integer.\n";
    }
}



void configureAgents(AppSession& session) {

    // A building must exist first.
    if (!session.building.has_value()) {

        std::cout << "\nNo valid building map found.\n";
        std::cout << "Create a building map first using option 1.\n";

        return;
    }

    // Agents cannot be changed after simulation initialization.
    if (session.simulation) {

        std::cout << "\nSimulation has already started.\n";
        std::cout << "Create a new building map to reset the session.\n";

        return;
    }

    // Create AgentManager on its first use.
    if (!session.agents) {

        const aeris::OccupancyGrid occupancy(*session.building);

        session.agents =
            std::make_unique<aeris::AgentManager>(occupancy);
    }

    // Determine how many additional agents can be placed.
    const auto statistics = aeris::analyzeMap(*session.building);

    const std::size_t availableCells =
        statistics.walkableCount - session.agents->size();

    if (availableCells == 0) {

        std::cout << "\nNo additional starting positions are available.\n";

        return;
    }

    std::cout << "\n========== AGENT CONFIGURATION ==========\n";

    std::cout << "Existing agents: "
              << session.agents->size() << '\n';

    int agentCount = 0;

    if (!readInteger("How many additional agents? ", agentCount)) {
        return;
    }

    if (agentCount < 1 || agentCount > 100 ||
        static_cast<std::size_t>(agentCount) > availableCells) {

        std::cout << "Invalid agent count. Available starting cells: "
                  << availableCells << '\n';

        return;
    }

    // Configure each agent.
    for (int i = 0; i < agentCount; ++i) {

        bool created = false;

        while (!created) {

            int row = 0;
            int column = 0;

            std::cout << "\nConfigure Agent "
                      << i + 1 << '\n';

            if (!readInteger("Starting row: ", row)) {
                return;
            }

            if (!readInteger("Starting column: ", column)) {
                return;
            }

            // Reject negative coordinates.
            if (row < 0 || column < 0) {

                std::cout << "Coordinates cannot be negative.\n";

                continue;
            }

            try {

                const std::size_t id =
                    session.agents->addAgent({
                        static_cast<std::size_t>(row),
                        static_cast<std::size_t>(column)
                    });

                std::cout << "Agent #" << id
                          << " created successfully.\n";

                created = true;

            } catch (const std::exception& error) {

                std::cout << "Cannot create agent: "
                          << error.what() << '\n';

                std::cout << "Please select another position.\n";
            }
        }
    }

    std::cout << "\n========== CURRENT AGENTS ==========\n";

    for (const aeris::Agent& agent : session.agents->getAgents()) {

        std::cout << "Agent #" << agent.id
                  << " | Position: ("
                  << agent.currentPosition.row << ", "
                  << agent.currentPosition.column << ")\n";
    }

    std::cout << "====================================\n";
}



const char* agentStatusName(aeris::AgentStatus status) {

    switch (status) {

        case aeris::AgentStatus::Ready:
            return "Ready";

        case aeris::AgentStatus::Evacuating:
            return "Evacuating";

        case aeris::AgentStatus::Evacuated:
            return "Evacuated";

        case aeris::AgentStatus::Trapped:
            return "Trapped";
    }

    return "Unknown";
}



void displayEvacuationResults(const AppSession& session) {

    if (!session.simulation) {

        std::cout << "\nNo evacuation results available.\n";
        return;
    }

    const auto result = session.simulation->results();

    std::cout << "\n========== EVACUATION RESULTS ==========\n\n";

    for (const aeris::Agent& agent : session.agents->getAgents()) {

        std::cout << "Agent #" << agent.id
                  << " | Status: "
                  << agentStatusName(agent.status)
                  << " | Position: ("
                  << agent.currentPosition.row << ", "
                  << agent.currentPosition.column << ")\n";
    }

    std::cout << "\n----------------------------------------\n";

    std::cout << "Total Agents:       "
              << result.totalAgents << '\n';

    std::cout << "Evacuated:          "
              << result.evacuatedAgents << '\n';

    std::cout << "Trapped:            "
              << result.trappedAgents << '\n';

    std::cout << "Remaining:          "
              << result.remainingAgents << '\n';

    std::cout << "Simulation Steps:   "
              << result.simulationSteps << '\n';

    if (session.simulationStalled) {

        std::cout << "\nWARNING: Simulation stalled due to movement conflicts.\n";

    } else if (session.simulationStepLimitReached) {

        std::cout << "\nWARNING: Maximum simulation step limit reached.\n";

    } else {

        std::cout << "\nSimulation completed.\n";
    }

    std::cout << "========================================\n";
}



void runEvacuationSimulation(AppSession& session) {

    if (!session.building.has_value()) {

        std::cout << "\nCreate a valid building map first.\n";
        return;
    }

    if (!session.agents || session.agents->size() == 0) {

        std::cout << "\nNo agents configured.\n";
        std::cout << "Add agents using option 2 first.\n";

        return;
    }

    if (session.simulation) {

        std::cout << "\nSimulation has already been executed.\n";

        displayEvacuationResults(session);
        return;
    }

    try {

        session.simulation =
            std::make_unique<aeris::EvacuationSimulation>(
                *session.building,
                *session.agents
            );

        // Calculate all evacuation routes.
        session.simulation->initialize();

        std::cout << "\n========== EVACUATION SIMULATION ==========\n";

        constexpr std::size_t MAX_STEPS = 1000;

        // Advance the simulation one tick at a time.
        while (!session.simulation->isComplete() &&
               session.simulation->results().simulationSteps < MAX_STEPS) {

            const bool moved = session.simulation->step();

            const auto current =
                session.simulation->results();

            std::cout << "\nSimulation Step: "
                      << current.simulationSteps << '\n';

            for (const aeris::Agent& agent : session.agents->getAgents()) {

                std::cout << "Agent #" << agent.id
                          << " | Position: ("
                          << agent.currentPosition.row << ", "
                          << agent.currentPosition.column << ")"
                          << " | " << agentStatusName(agent.status)
                          << '\n';
            }

            // Stop if no agent can make progress.
            if (!moved && !session.simulation->isComplete()) {

                session.simulationStalled = true;
                break;
            }
        }

        // Determine whether the simulation reached its step limit.
        session.simulationStepLimitReached =
            !session.simulation->isComplete() &&
            !session.simulationStalled &&
            session.simulation->results().simulationSteps >= MAX_STEPS;

        std::cout << "\n============================================\n";

        displayEvacuationResults(session);

    } catch (const std::exception& error) {

        std::cout << "\nSimulation error: "
                  << error.what() << '\n';

        session.simulation.reset();
    }
}




int main() {
    int choice = -1;
    AppSession session;

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
                 createManualBuildingMap(session);
                 break;

            case 2:
                configureAgents(session);
                break;

            case 3:
                runEvacuationSimulation(session);
                break;

            case 4:
                displayEvacuationResults(session);
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
