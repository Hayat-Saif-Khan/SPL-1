
#include "EvacuationSimulation.hpp"

#include <iostream>
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

    if (layout.empty() || layout.front().empty()) {
        throw std::invalid_argument("Test layout cannot be empty.");
    }

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

void testSingleAgentEvacuation() {

    auto grid = makeGrid({
        "#####",
        "#..E#",
        "#####"
    });

    aeris::OccupancyGrid occupancy(grid);
    aeris::AgentManager manager(occupancy);

    const std::size_t id = manager.addAgent({1, 1});

    aeris::EvacuationSimulation simulation(grid, manager);

    const auto result = simulation.run();

    check(
        result.evacuatedAgents == 1,
        "Single agent reaches the exit"
    );

    check(
        manager.getAgent(id).status ==
            aeris::AgentStatus::Evacuated,
        "Successfully evacuated agent receives Evacuated status"
    );

    check(
        manager.getAgent(id).currentPosition ==
            aeris::GridPosition{1, 3},
        "Agent finishes at the exit position"
    );

    check(
        result.simulationSteps == 2,
        "Simulation counts two movement steps"
    );
}

void testStepByStepMovement() {

    auto grid = makeGrid({
        "#####",
        "#..E#",
        "#####"
    });

    aeris::OccupancyGrid occupancy(grid);
    aeris::AgentManager manager(occupancy);

    const std::size_t id = manager.addAgent({1, 1});

    aeris::EvacuationSimulation simulation(grid, manager);

    simulation.initialize();

    // First tick.
    check(
        simulation.step(),
        "First tick moves the agent"
    );

    check(
        manager.getAgent(id).currentPosition ==
            aeris::GridPosition{1, 2},
        "Agent moves exactly one cell in the first tick"
    );

    check(
        manager.getAgent(id).status ==
            aeris::AgentStatus::Evacuating,
        "Agent is marked Evacuating during movement"
    );

    // Second tick.
    check(
        simulation.step(),
        "Second tick moves the agent to the exit"
    );

    check(
        manager.getAgent(id).status ==
            aeris::AgentStatus::Evacuated,
        "Agent is evacuated after reaching the exit"
    );

    check(
        simulation.isComplete(),
        "Simulation completes after agent evacuation"
    );

    check(
        !simulation.step(),
        "Completed simulation does not move agents again"
    );
}

void testUnreachableAgent() {

    auto grid = makeGrid({
        "########",
        "#..#...#",
        "#..#...#",
        "####E###",
        "########"
    });

    aeris::OccupancyGrid occupancy(grid);
    aeris::AgentManager manager(occupancy);

    const std::size_t id = manager.addAgent({1, 1});

    aeris::EvacuationSimulation simulation(grid, manager);

    const auto result = simulation.run();

    check(
        manager.getAgent(id).status == aeris::AgentStatus::Trapped,
        "Agent without a reachable exit becomes Trapped"
    );

    check(
        result.trappedAgents == 1,
        "Trapped agent is included in simulation results"
    );

    check(
        result.isComplete(),
        "Simulation completes when all agents are trapped"
    );

    check(
        result.simulationSteps == 0,
        "Unreachable agent requires no movement steps"
    );
}

void testMultipleAgents() {

    auto grid = makeGrid({
        "#######",
        "#....E#",
        "#.....#",
        "#.....#",
        "#######"
    });

    aeris::OccupancyGrid occupancy(grid);
    aeris::AgentManager manager(occupancy);

    const std::size_t firstId =
        manager.addAgent({1, 1});

    const std::size_t secondId =
        manager.addAgent({3, 1});

    aeris::EvacuationSimulation simulation(grid, manager);

    const auto result = simulation.run();

    check(
        result.totalAgents == 2,
        "Simulation manages two agents"
    );

    check(
        result.evacuatedAgents == 2,
        "Both agents successfully evacuate"
    );

    check(
        manager.getAgent(firstId).status ==
            aeris::AgentStatus::Evacuated &&
        manager.getAgent(secondId).status ==
            aeris::AgentStatus::Evacuated,
        "Both agents receive Evacuated status"
    );

    check(
        result.isComplete(),
        "Multi-agent simulation completes"
    );
}

void testAgentAlreadyAtExit() {

    auto grid = makeGrid({
        "###",
        "#E#",
        "###"
    });

    aeris::OccupancyGrid occupancy(grid);
    aeris::AgentManager manager(occupancy);

    const std::size_t id = manager.addAgent({1, 1});

    aeris::EvacuationSimulation simulation(grid, manager);

    const auto result = simulation.run();

    check(
        manager.getAgent(id).status ==
            aeris::AgentStatus::Evacuated,
        "Agent starting at an exit is immediately evacuated"
    );

    check(
        result.simulationSteps == 0,
        "Agent at exit requires zero movement steps"
    );
}

void testMaximumStepLimit() {

    auto grid = makeGrid({
        "######",
        "#....E",
        "######"
    });

    aeris::OccupancyGrid occupancy(grid);
    aeris::AgentManager manager(occupancy);

    const std::size_t id = manager.addAgent({1, 1});

    aeris::EvacuationSimulation simulation(grid, manager);

    const auto result = simulation.run(1);

    check(
        result.simulationSteps == 1,
        "Simulation respects maximum step limit"
    );

    check(
        result.remainingAgents == 1,
        "Agent remains when simulation step limit is reached"
    );

    check(
        manager.getAgent(id).status ==
            aeris::AgentStatus::Evacuating,
        "Incomplete agent retains Evacuating status"
    );

    check(
        !result.isComplete(),
        "Simulation reports incomplete evacuation"
    );
}

void testInitializationRules() {

    auto grid = makeGrid({
        "#####",
        "#..E#",
        "#####"
    });

    aeris::OccupancyGrid occupancy(grid);
    aeris::AgentManager manager(occupancy);

    manager.addAgent({1, 1});

    aeris::EvacuationSimulation simulation(grid, manager);

    bool stepBeforeInitializationRejected = false;

    try {
        simulation.step();
    } catch (const std::logic_error&) {
        stepBeforeInitializationRejected = true;
    }

    check(
        stepBeforeInitializationRejected,
        "Simulation cannot advance before initialization"
    );

    simulation.initialize();

    bool repeatedInitializationRejected = false;

    try {
        simulation.initialize();
    } catch (const std::logic_error&) {
        repeatedInitializationRejected = true;
    }

    check(
        repeatedInitializationRejected,
        "Simulation cannot be initialized twice"
    );
}

int main() {

    std::cout << "\n========== EVACUATION SIMULATION TESTS ==========\n\n";

    testSingleAgentEvacuation();
    testStepByStepMovement();
    testUnreachableAgent();
    testMultipleAgents();
    testAgentAlreadyAtExit();
    testMaximumStepLimit();
    testInitializationRules();

    std::cout << "\n========== TEST SUMMARY ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "==================================\n";

    return failed == 0 ? 0 : 1;
}
