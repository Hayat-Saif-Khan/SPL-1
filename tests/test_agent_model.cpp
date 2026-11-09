
#include "Agent.hpp"

#include <iostream>
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

    aeris::Agent agent;

    // Test 1: Agent ID.
    agent.id = 1;

    check(
        agent.id == 1,
        "Agent ID is stored correctly"
    );

    // Test 2: Starting position.
    agent.startPosition = {2, 3};

    check(
        agent.startPosition == aeris::GridPosition{2, 3},
        "Agent starting position is stored correctly"
    );

    // Test 3: Current position.
    agent.currentPosition = agent.startPosition;

    check(
        agent.currentPosition == agent.startPosition,
        "Agent current position is initialized correctly"
    );

    // Test 4: Initial status.
    check(
        agent.status == aeris::AgentStatus::Ready,
        "New agent has Ready status"
    );

    // Test 5: Agent movement.
    agent.currentPosition = {2, 4};

    check(
        agent.currentPosition == aeris::GridPosition{2, 4},
        "Agent current position can be updated"
    );

    check(
        agent.startPosition == aeris::GridPosition{2, 3},
        "Agent starting position remains unchanged"
    );

    // Test 6: Status transition.
    agent.status = aeris::AgentStatus::Evacuating;

    check(
        agent.status == aeris::AgentStatus::Evacuating,
        "Agent status can change to Evacuating"
    );

    agent.status = aeris::AgentStatus::Evacuated;

    check(
        agent.status == aeris::AgentStatus::Evacuated,
        "Agent status can change to Evacuated"
    );

    agent.status = aeris::AgentStatus::Trapped;

    check(
        agent.status == aeris::AgentStatus::Trapped,
        "Agent status can change to Trapped"
    );

    std::cout << "\n========== AGENT MODEL TESTS ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "=======================================\n";

    return failed == 0 ? 0 : 1;
}
