
#pragma once

#include "GridPosition.hpp"

#include <cstddef>

namespace aeris {

// Represents an evacuation agent's current state.
enum class AgentStatus {
    Ready,
    Evacuating,
    Evacuated,
    Trapped
};

struct Agent {

    // Unique identifier assigned by the agent manager.
    std::size_t id{0};

    // Original position when the agent enters the simulation.
    GridPosition startPosition{};

    // Current position during evacuation.
    GridPosition currentPosition{};

    // Current evacuation status.
    AgentStatus status{AgentStatus::Ready};
};

}
