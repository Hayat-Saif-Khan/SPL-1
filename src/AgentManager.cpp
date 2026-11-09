
#include "AgentManager.hpp"

#include <algorithm>
#include <stdexcept>

namespace aeris {

// Initialize the manager with an occupancy grid.
AgentManager::AgentManager(const OccupancyGrid& occupancyGrid)
    : occupancyGrid_(occupancyGrid) {
}

// Check whether an agent already occupies a position.
bool AgentManager::isOccupied(GridPosition position) const noexcept {

    for (const Agent& agent : agents_) {

        // Evacuated agents no longer occupy the building.
        if (agent.status != AgentStatus::Evacuated &&
            agent.currentPosition == position) {

            return true;
        }
    }

    return false;
}

// Create an agent at a validated starting position.
std::size_t AgentManager::addAgent(GridPosition startPosition) {

    // Check building boundaries.
    if (startPosition.row >= occupancyGrid_.rowCount() ||
        startPosition.column >= occupancyGrid_.columnCount()) {

        throw std::out_of_range(
            "Agent starting position is outside the building."
        );
    }

    // Check whether the position is traversable.
    if (occupancyGrid_.at(
            startPosition.row,
            startPosition.column
        ) != OccupancyState::Free) {

        throw std::invalid_argument(
            "Agent cannot start in a blocked cell."
        );
    }

    // Prevent multiple agents from sharing a starting cell.
    if (isOccupied(startPosition)) {

        throw std::invalid_argument(
            "Another agent already occupies this position."
        );
    }

    // Prevent agent ID overflow.
    if (nextId_ == 0) {
        throw std::overflow_error("Agent ID limit reached.");
    }

    // Create and configure the agent.
    Agent agent;

    agent.id = nextId_;
    agent.startPosition = startPosition;
    agent.currentPosition = startPosition;
    agent.status = AgentStatus::Ready;

    agents_.push_back(agent);

    ++nextId_;

    return agent.id;
}

// Retrieve an agent by ID.
const Agent& AgentManager::getAgent(std::size_t id) const {

    const auto it = std::find_if(
        agents_.begin(),
        agents_.end(),
        [id](const Agent& agent) {
            return agent.id == id;
        }
    );

    if (it == agents_.end()) {
        throw std::out_of_range(
            "No agent exists with the requested ID."
        );
    }

    return *it;
}

// Return all agents.
const std::vector<Agent>& AgentManager::getAgents() const noexcept {
    return agents_;
}

// Return the number of agents.
std::size_t AgentManager::size() const noexcept {
    return agents_.size();
}

}
