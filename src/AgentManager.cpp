
#include "AgentManager.hpp"

#include <algorithm>
#include <stdexcept>

namespace aeris {

AgentManager::AgentManager(const OccupancyGrid& occupancyGrid)
    : occupancyGrid_(occupancyGrid) {
}

// Checks whether another active agent occupies a position.
bool AgentManager::isOccupied(GridPosition position) const noexcept {

    for (const Agent& agent : agents_) {

        if (agent.status != AgentStatus::Evacuated &&
            agent.currentPosition == position) {

            return true;
        }
    }

    return false;
}

// Finds an agent and returns a modifiable reference.
Agent& AgentManager::findAgent(std::size_t id) {

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

// Creates an agent at a validated starting position.
std::size_t AgentManager::addAgent(GridPosition startPosition) {

    if (startPosition.row >= occupancyGrid_.rowCount() ||
        startPosition.column >= occupancyGrid_.columnCount()) {

        throw std::out_of_range(
            "Agent starting position is outside the building."
        );
    }

    if (occupancyGrid_.at(
            startPosition.row,
            startPosition.column
        ) != OccupancyState::Free) {

        throw std::invalid_argument(
            "Agent cannot start in a blocked cell."
        );
    }

    if (isOccupied(startPosition)) {

        throw std::invalid_argument(
            "Another agent already occupies this position."
        );
    }

    if (nextId_ == 0) {
        throw std::overflow_error("Agent ID limit reached.");
    }

    Agent agent;

    agent.id = nextId_;
    agent.startPosition = startPosition;
    agent.currentPosition = startPosition;
    agent.status = AgentStatus::Ready;

    agents_.push_back(agent);

    ++nextId_;

    return agent.id;
}

// Retrieves an agent using its ID.
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

// Moves an agent one cell at a time.
bool AgentManager::moveAgent(
    std::size_t id,
    GridPosition nextPosition
) {

    Agent& agent = findAgent(id);

    // Terminal agents cannot move.
    if (agent.status == AgentStatus::Evacuated ||
        agent.status == AgentStatus::Trapped) {

        throw std::logic_error(
            "An evacuated or trapped agent cannot move."
        );
    }

    // Check building boundaries.
    if (nextPosition.row >= occupancyGrid_.rowCount() ||
        nextPosition.column >= occupancyGrid_.columnCount()) {

        throw std::out_of_range(
            "Movement destination is outside the building."
        );
    }

    // Movement must be horizontally or vertically adjacent.
    const std::size_t rowDifference =
        nextPosition.row > agent.currentPosition.row
            ? nextPosition.row - agent.currentPosition.row
            : agent.currentPosition.row - nextPosition.row;

    const std::size_t columnDifference =
        nextPosition.column > agent.currentPosition.column
            ? nextPosition.column - agent.currentPosition.column
            : agent.currentPosition.column - nextPosition.column;

    if (rowDifference + columnDifference != 1) {

        throw std::invalid_argument(
            "Agent can move only to an adjacent cell."
        );
    }

    // The destination must be traversable.
    if (occupancyGrid_.at(
            nextPosition.row,
            nextPosition.column
        ) != OccupancyState::Free) {

        throw std::invalid_argument(
            "Agent cannot move into a blocked cell."
        );
    }

    // Prevent collisions with other active agents.
    if (isOccupied(nextPosition)) {
        return false;
    }

    agent.currentPosition = nextPosition;
    agent.status = AgentStatus::Evacuating;

    return true;
}

// Marks an agent as successfully evacuated.
void AgentManager::markEvacuated(std::size_t id) {

    Agent& agent = findAgent(id);

    if (agent.status == AgentStatus::Trapped) {
        throw std::logic_error(
            "A trapped agent cannot be marked evacuated."
        );
    }

    agent.status = AgentStatus::Evacuated;
}

// Marks an agent as trapped.
void AgentManager::markTrapped(std::size_t id) {

    Agent& agent = findAgent(id);

    if (agent.status == AgentStatus::Evacuated) {
        throw std::logic_error(
            "An evacuated agent cannot be marked trapped."
        );
    }

    agent.status = AgentStatus::Trapped;
}

// Returns all managed agents.
const std::vector<Agent>& AgentManager::getAgents() const noexcept {
    return agents_;
}

// Returns the number of managed agents.
std::size_t AgentManager::size() const noexcept {
    return agents_.size();
}

}
