
#pragma once

#include "Agent.hpp"
#include "OccupancyGrid.hpp"

#include <cstddef>
#include <vector>

namespace aeris {

class AgentManager {

public:

    explicit AgentManager(const OccupancyGrid& occupancyGrid);

    // Creates an agent at a validated starting position.
    std::size_t addAgent(GridPosition startPosition);

    // Retrieves an agent using its ID.
    const Agent& getAgent(std::size_t id) const;

    // Returns all managed agents.
    const std::vector<Agent>& getAgents() const noexcept;

    // Returns the number of managed agents.
    std::size_t size() const noexcept;

    // Moves an agent to an adjacent free cell.
    // Returns false when another active agent occupies the cell.
    bool moveAgent(std::size_t id, GridPosition nextPosition);

    // Changes the status of an agent after evacuation.
    void markEvacuated(std::size_t id);

    // Marks an agent that cannot reach an exit.
    void markTrapped(std::size_t id);

private:

    OccupancyGrid occupancyGrid_;

    std::vector<Agent> agents_;

    std::size_t nextId_{1};

    bool isOccupied(GridPosition position) const noexcept;

    // Provides internal mutable access without exposing the agent vector.
    Agent& findAgent(std::size_t id);
};

}
