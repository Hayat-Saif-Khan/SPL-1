
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

    aeris::OccupancyGrid occupancy(building);

    aeris::AgentManager manager(occupancy);

    // Test 1: Initially empty.
    check(manager.size() == 0,
          "AgentManager starts empty");

    // Test 2: Create first agent.
    const std::size_t firstId =
        manager.addAgent({1, 1});

    check(firstId == 1,
          "First agent receives ID 1");

    // Test 3: Create second agent.
    const std::size_t secondId =
        manager.addAgent({1, 2});

    check(secondId == 2,
          "Second agent receives unique ID 2");

    // Test 4: Starting position.
    const aeris::Agent& firstAgent =
        manager.getAgent(firstId);

    check(
        firstAgent.startPosition == aeris::GridPosition{1, 1},
        "Agent starting position is stored correctly"
    );

    // Test 5: Current position.
    check(
        firstAgent.currentPosition == aeris::GridPosition{1, 1},
        "Agent current position is initialized correctly"
    );

    // Test 6: Initial status.
    check(
        firstAgent.status == aeris::AgentStatus::Ready,
        "New agent has Ready status"
    );

    // Test 7: Agent retrieval.
    check(
        manager.getAgent(secondId).id == secondId,
        "Agent can be retrieved using its ID"
    );

    // Test 8: Agent count.
    check(
        manager.size() == 2 &&
        manager.getAgents().size() == 2,
        "AgentManager stores both agents"
    );

    // Test 9: Reject blocked starting position.
    bool blockedRejected = false;

    try {
        manager.addAgent({0, 0});
    } catch (const std::invalid_argument&) {
        blockedRejected = true;
    }

    check(blockedRejected,
          "Agent cannot start inside a wall");

    // Test 10: Reject out-of-bounds starting position.
    bool outOfBoundsRejected = false;

    try {
        manager.addAgent({4, 0});
    } catch (const std::out_of_range&) {
        outOfBoundsRejected = true;
    }

    check(outOfBoundsRejected,
          "Agent cannot start outside the building");

    // Test 11: Reject duplicate starting position.
    bool duplicateRejected = false;

    try {
        manager.addAgent({1, 1});
    } catch (const std::invalid_argument&) {
        duplicateRejected = true;
    }

    check(duplicateRejected,
          "Two agents cannot share a starting position");

    // Test 12: Rejected operations do not add agents.
    check(manager.size() == 2,
          "Rejected operations do not modify agent count");

    // Test 13: Prevent collision with another agent.
    check(
        !manager.moveAgent(firstId, {1, 2}),
        "Agent waits when another agent occupies its destination"
    );

    // Test 14: Move second agent into a free adjacent cell.
    check(
        manager.moveAgent(secondId, {2, 2}),
        "Agent can move into an adjacent free cell"
    );

    // Test 15: Movement changes agent status.
    check(
        manager.getAgent(secondId).status ==
            aeris::AgentStatus::Evacuating,
        "Moving agent receives Evacuating status"
    );

    // Test 16: First agent can enter the cell vacated by agent 2.
    check(
        manager.moveAgent(firstId, {1, 2}),
        "Agent can enter a cell vacated by another agent"
    );

    // Test 17: Position is updated correctly.
    check(
        manager.getAgent(firstId).currentPosition ==
            aeris::GridPosition{1, 2},
        "Agent position updates after movement"
    );

    // Test 18: Reject movement into a wall.
    bool blockedMovementRejected = false;

    try {
        manager.moveAgent(secondId, {2, 1});
    } catch (const std::invalid_argument&) {
        blockedMovementRejected = true;
    }

    check(blockedMovementRejected,
          "Agent cannot move into a wall");

    // Test 19: Reject out-of-bounds movement.
    bool invalidMovementRejected = false;

    try {
        manager.moveAgent(secondId, {4, 2});
    } catch (const std::out_of_range&) {
        invalidMovementRejected = true;
    }

    check(invalidMovementRejected,
          "Agent cannot move outside the building");

    // Test 20: Reject non-adjacent movement.
    bool nonAdjacentRejected = false;

    try {
        manager.moveAgent(secondId, {1, 1});
    } catch (const std::invalid_argument&) {
        nonAdjacentRejected = true;
    }

    check(nonAdjacentRejected,
          "Agent cannot jump over cells");

    // Test 21: Mark first agent as evacuated.
    manager.markEvacuated(firstId);

    check(
        manager.getAgent(firstId).status ==
            aeris::AgentStatus::Evacuated,
        "Agent can be marked Evacuated"
    );

    // Test 22: Mark second agent as evacuated.
    manager.markEvacuated(secondId);

    check(
        manager.getAgent(secondId).status ==
            aeris::AgentStatus::Evacuated,
        "Second agent can be marked Evacuated"
    );

    // Test 23: Evacuated agents no longer occupy their cells.
    const std::size_t thirdId =
        manager.addAgent({2, 2});

    check(
        thirdId == 3 && manager.size() == 3,
        "A cell can be reused after its agent evacuates"
    );

    // Test 24: Mark third agent as trapped.
    manager.markTrapped(thirdId);

    check(
        manager.getAgent(thirdId).status ==
            aeris::AgentStatus::Trapped,
        "Agent can be marked Trapped"
    );

    // Test 25: Terminal agents cannot move.
    bool trappedMovementRejected = false;

    try {
        manager.moveAgent(thirdId, {2, 2});
    } catch (const std::logic_error&) {
        trappedMovementRejected = true;
    }

    check(
        trappedMovementRejected,
        "Trapped agent cannot move"
    );

    // Test 26: A trapped agent cannot be marked evacuated.
    bool invalidTransitionRejected = false;

    try {
        manager.markEvacuated(thirdId);
    } catch (const std::logic_error&) {
        invalidTransitionRejected = true;
    }

    check(
        invalidTransitionRejected,
        "Trapped agent cannot become Evacuated"
    );

    std::cout << "\n========== AGENT MANAGER TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "=========================================\n";

    return failed == 0 ? 0 : 1;
}
