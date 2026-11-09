
#include "OccupancyGrid.hpp"

namespace aeris {

OccupancyGrid::OccupancyGrid(const BuildingGrid& building) {

    const std::size_t rows = building.rowCount();
    const std::size_t columns = building.columnCount();

    // Initialize all cells as blocked.
    cells_.resize(
        rows,
        std::vector<OccupancyState>(
            columns,
            OccupancyState::Blocked
        )
    );

    // Convert building cells into occupancy states.
    for (std::size_t row = 0; row < rows; ++row) {

        for (std::size_t column = 0;
             column < columns;
             ++column) {

            const CellType type = building.at(row, column).type;

            switch (type) {

                case CellType::Wall:
                    cells_[row][column] = OccupancyState::Blocked;
                    break;

                case CellType::Floor:
                case CellType::Door:
                case CellType::Exit:
                    cells_[row][column] = OccupancyState::Free;
                    break;
            }
        }
    }
}


std::size_t OccupancyGrid::rowCount() const noexcept {
    return cells_.size();
}


std::size_t OccupancyGrid::columnCount() const noexcept {
    return cells_.front().size();
}

// Returns the occupancy state using bounds-checked access.
OccupancyState OccupancyGrid::at(
    std::size_t row,
    std::size_t column
) const {

    return cells_.at(row).at(column);
}

}
