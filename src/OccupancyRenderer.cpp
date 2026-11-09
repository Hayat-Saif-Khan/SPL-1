
#include "OccupancyRenderer.hpp"

#include <ostream>

namespace aeris {

namespace {

// Converts an occupancy state into a display symbol.
char symbolFor(OccupancyState state) {

    switch (state) {

        case OccupancyState::Blocked:
            return '#';

        case OccupancyState::Free:
            return '.';
    }

    return '?';
}

}

// Displays the occupancy grid.
void renderOccupancyGrid(
    const OccupancyGrid& grid,
    std::ostream& output
) {

    for (std::size_t row = 0;
         row < grid.rowCount();
         ++row) {

        for (std::size_t column = 0;
             column < grid.columnCount();
             ++column) {

            output << symbolFor(grid.at(row, column));
        }

        output << '\n';
    }
}

}
