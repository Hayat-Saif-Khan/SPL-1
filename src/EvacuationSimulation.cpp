
#include "EvacuationSimulation.hpp"

#include <stdexcept>
#include <utility>

namespace aeris {

EvacuationSimulation::EvacuationSimulation(
    const BuildingGrid& grid,
    AgentManager& manager
)
    : grid_(grid), manager_(manager) {
}

// Calculates an evacuation route for every agent.
void EvacuationSimulation::initialize() {

    if (initialized_) {
        throw std::logic_error(
            "Evacuation simulation is already initialized."
        );
    }

    // This simulation expects newly created agents.
    for (const Agent& agent : manager_.getAgents()) {

        if (agent.status != AgentStatus::Ready) {
            throw std::logic_error(
                "All agents must be Ready before initialization."
            );
        }
    }

    for (const Agent& agent : manager_.getAgents()) {

        // Find the nearest reachable exit.
        auto path = findEvacuationPath(
            grid_,
            agent.currentPosition
        );

        // No route means the agent is trapped.
        if (!path.has_value()) {

            manager_.markTrapped(agent.id);
        }

        // An agent already at an exit is immediately evacuated.
        else if (path->size() == 1) {

            manager_.markEvacuated(agent.id);
        }

        // Store the route for future simulation steps.
        else {

            routes_.emplace(
                agent.id,
                RouteProgress{
                    std::move(*path),
                    1
                }
            );
        }
    }

    initialized_ = true;
}

// Advances every active agent by at most one cell.
bool EvacuationSimulation::step() {

    if (!initialized_) {
        throw std::logic_error(
            "Initialize the simulation before advancing it."
        );
    }

    if (isComplete()) {
        return false;
    }

    ++stepCount_;

    bool movedAnyAgent = false;

    // Agents are processed in their creation/ID order.
    for (const Agent& agent : manager_.getAgents()) {

        const std::size_t id = agent.id;

        // Ignore agents that have reached a terminal status.
        if (agent.status == AgentStatus::Evacuated ||
            agent.status == AgentStatus::Trapped) {

            continue;
        }

        auto routeIt = routes_.find(id);

        if (routeIt == routes_.end()) {
            throw std::logic_error(
                "Active agent has no assigned evacuation route."
            );
        }

        RouteProgress& progress = routeIt->second;

        // Get the next position in the assigned route.
        const GridPosition nextPosition =
            progress.path[progress.nextIndex];

        // Try to move the agent.
        // It waits if another active agent occupies the destination.
        const bool moved = manager_.moveAgent(
            id,
            nextPosition
        );

        if (!moved) {
            continue;
        }

        movedAnyAgent = true;

        // Advance the route position.
        ++progress.nextIndex;

        // Reaching the final route position means reaching an exit.
        if (progress.nextIndex == progress.path.size()) {

            manager_.markEvacuated(id);
        }
    }

    return movedAnyAgent;
}

// Runs the simulation until completion, a stall, or the tick limit.
EvacuationResults EvacuationSimulation::run(
    std::size_t maxSteps
) {

    if (!initialized_) {
        initialize();
    }

    stalled_ = false;

    for (std::size_t stepNumber = 0;
         stepNumber < maxSteps && !isComplete();
         ++stepNumber) {

        const bool moved = step();

        // No agent could move, so the simulation is stalled.
        if (!moved && !isComplete()) {

            stalled_ = true;
            break;
        }
    }

    return results();
}

// Checks whether every agent has reached a terminal status.
bool EvacuationSimulation::isComplete() const noexcept {

    for (const Agent& agent : manager_.getAgents()) {

        if (agent.status == AgentStatus::Ready ||
            agent.status == AgentStatus::Evacuating) {

            return false;
        }
    }

    return true;
}

// Calculates the current evacuation statistics.
EvacuationResults EvacuationSimulation::results() const noexcept {

    EvacuationResults result;

    const std::vector<Agent>& agents = manager_.getAgents();

    result.totalAgents = agents.size();
    result.simulationSteps = stepCount_;
    result.stalled = stalled_;

    for (const Agent& agent : agents) {

        switch (agent.status) {

            case AgentStatus::Ready:
            case AgentStatus::Evacuating:
                ++result.remainingAgents;
                break;

            case AgentStatus::Evacuated:
                ++result.evacuatedAgents;
                break;

            case AgentStatus::Trapped:
                ++result.trappedAgents;
                break;
        }
    }

    return result;
}

}
