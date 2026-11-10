
#pragma once

#include "AgentManager.hpp"
#include "Pathfinder.hpp"

#include <cstddef>
#include <unordered_map>

namespace aeris {

// Stores the final simulation statistics.
struct EvacuationResults {

    std::size_t totalAgents{0};
    std::size_t evacuatedAgents{0};
    std::size_t trappedAgents{0};
    std::size_t remainingAgents{0};
    std::size_t simulationSteps{0};

    bool stalled{false};

    bool isComplete() const noexcept {
        return remainingAgents == 0;
    }
};

class EvacuationSimulation {

public:

    // Uses the building layout and an existing agent manager.
    EvacuationSimulation(
        const BuildingGrid& grid,
        AgentManager& manager
    );

    // Calculates and assigns routes to agents.
    void initialize();

    // Advances the simulation by one tick.
    // Returns true if at least one agent moved.
    bool step();

    // Runs the simulation up to the specified number of ticks.
    EvacuationResults run(std::size_t maxSteps = 1000);

    // Checks whether all agents have reached a terminal status.
    bool isComplete() const noexcept;

    // Returns current simulation statistics.
    EvacuationResults results() const noexcept;

private:

    struct RouteProgress {

        EvacuationPath path;

        // The next position the agent should visit.
        std::size_t nextIndex{1};
    };

    BuildingGrid grid_;

    AgentManager& manager_;

    std::unordered_map<std::size_t, RouteProgress> routes_;

    std::size_t stepCount_{0};

    bool initialized_{false};
    bool stalled_{false};
};

}
