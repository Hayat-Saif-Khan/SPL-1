
#pragma once

#include "Agent.hpp"
#include "OccupancyGrid.hpp"

#include <cstddef>
#include <vector>

namespace aeris {

class AgentManager {

public:

    // Creates a manager using an occupancy grid snapshot.
    explicit AgentManager(const OccupancyGrid& occupancyGrid);

    // Creates an agent at a validated starting position.
    std::size_t addAgent(GridPosition startPosition);

    // Retrieves an agent using its unique ID.
    const Agent& getAgent(std::size_t id) const;

    // Returns all managed agents.
    const std::vector<Agent>& getAgents() const noexcept;

    // Returns the number of managed agents.
    std::size_t size() const noexcept;

private:

    OccupancyGrid occupancyGrid_;

    std::vector<Agent> agents_;

    std::size_t nextId_{1};

    // Checks whether a position is already occupied.
    bool isOccupied(GridPosition position) const noexcept;
};

}
