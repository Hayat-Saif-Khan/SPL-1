
#pragma once

#include "BuildingGrid.hpp"

#include <cstddef>
#include <vector>

namespace aeris {

enum class OccupancyState {
    Blocked,
    Free
};

class OccupancyGrid {
public:

    // Creates an occupancy grid from a building grid.
    explicit OccupancyGrid(const BuildingGrid& building);

    // Returns grid dimensions.
    std::size_t rowCount() const noexcept;
    std::size_t columnCount() const noexcept;

    // Returns the occupancy state of a cell.
    OccupancyState at(
        std::size_t row,
        std::size_t column
    ) const;

private:

    std::vector<std::vector<OccupancyState>> cells_;
};

}
