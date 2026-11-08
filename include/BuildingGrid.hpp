
#pragma once

#include "Cell.hpp"

#include <cstddef>
#include <vector>

namespace aeris {

class BuildingGrid {
public:
    // Creates a grid with the specified dimensions.
    BuildingGrid(std::size_t rows, std::size_t columns);

    // Returns the grid dimensions.
    std::size_t rowCount() const noexcept;
    std::size_t columnCount() const noexcept;

    // Provides access to an individual cell.
    Cell& at(std::size_t row, std::size_t column);
    const Cell& at(std::size_t row, std::size_t column) const;

private:
    std::vector<std::vector<Cell>> cells_;
};

}
