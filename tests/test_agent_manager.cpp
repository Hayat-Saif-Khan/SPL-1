
#include "AgentManager.hpp"

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

    // Create a building and configure traversable cells.
    aeris::BuildingGrid building(4, 5);

    building.at(1, 1).type = aeris::CellType::Floor;
    building.at(1, 2).type = aeris::CellType::Door;
    building.at(2, 2).type = aeris::CellType::Floor;

    // Generate occupancy representation.
    aeris::OccupancyGrid occupancy(building);

    // Initialize AgentManager.
    aeris::AgentManager manager(occupancy);

    // Test 1: Initially empty.
    check(
        manager.size() == 0,
        "AgentManager starts empty"
    );

    // Test 2: Create first agent.
    const std::size_t firstId =
        manager.addAgent({1, 1});

    check(
        firstId == 1,
        "First agent receives ID 1"
    );

    // Test 3: Create second agent.
    const std::size_t secondId =
        manager.addAgent({1, 2});

    check(
        secondId == 2,
        "Second agent receives unique ID 2"
    );

    // Test 4: Verify agent position.
    const aeris::Agent& firstAgent =
        manager.getAgent(firstId);

    check(
        firstAgent.startPosition == aeris::GridPosition{1, 1},
        "Agent starting position is stored correctly"
    );

    check(
        firstAgent.currentPosition == aeris::GridPosition{1, 1},
        "Agent current position is initialized correctly"
    );

    // Test 5: Verify initial status.
    check(
        firstAgent.status == aeris::AgentStatus::Ready,
        "New agent has Ready status"
    );

    // Test 6: Agent retrieval.
    check(
        manager.getAgent(secondId).id == secondId,
        "Agent can be retrieved using its ID"
    );

    // Test 7: Agent count.
    check(
        manager.size() == 2 &&
        manager.getAgents().size() == 2,
        "AgentManager stores both agents"
    );

    // Test 8: Reject blocked starting position.
    bool blockedRejected = false;

    try {
        manager.addAgent({0, 0});
    } catch (const std::invalid_argument&) {
        blockedRejected = true;
    }

    check(
        blockedRejected,
        "Agent cannot start inside a wall"
    );

    // Test 9: Reject out-of-bounds position.
    bool outOfBoundsRejected = false;

    try {
        manager.addAgent({4, 0});
    } catch (const std::out_of_range&) {
        outOfBoundsRejected = true;
    }

    check(
        outOfBoundsRejected,
        "Agent cannot start outside the building"
    );

    // Test 10: Reject duplicate starting position.
    bool duplicateRejected = false;

    try {
        manager.addAgent({1, 1});
    } catch (const std::invalid_argument&) {
        duplicateRejected = true;
    }

    check(
        duplicateRejected,
        "Two agents cannot share a starting position"
    );

    // Test 11: Reject unknown agent ID.
    bool unknownIdRejected = false;

    try {
        (void)manager.getAgent(999);
    } catch (const std::out_of_range&) {
        unknownIdRejected = true;
    }

    check(
        unknownIdRejected,
        "Unknown agent ID is rejected"
    );

    // Verify rejected operations did not add agents.
    check(
        manager.size() == 2,
        "Rejected operations do not modify agent count"
    );

    std::cout << "\n========== AGENT MANAGER TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "=========================================\n";

    return failed == 0 ? 0 : 1;
}