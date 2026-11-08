
#include "BuildingGrid.hpp"

#include <stdexcept>

namespace aeris {

// Constructs a rectangular building grid.
BuildingGrid::BuildingGrid(std::size_t rows, std::size_t columns) {

    // A grid must have positive dimensions.
    if (rows == 0 || columns == 0) {
        throw std::invalid_argument(
            "Building grid dimensions must be greater than zero."
        );
    }

    // Create the specified number of rows and columns.
    cells_.resize(rows, std::vector<Cell>(columns));
}

// Returns the number of rows.
std::size_t BuildingGrid::rowCount() const noexcept {
    return cells_.size();
}

// Returns the number of columns.
std::size_t BuildingGrid::columnCount() const noexcept {
    return cells_.front().size();
}

// Returns a modifiable reference to a cell.
Cell& BuildingGrid::at(std::size_t row, std::size_t column) {
    return cells_.at(row).at(column);
}

// Returns a read-only reference to a cell.
const Cell& BuildingGrid::at(
    std::size_t row,
    std::size_t column
) const {
    return cells_.at(row).at(column);
}

}
