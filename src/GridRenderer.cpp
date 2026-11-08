
#include "GridRenderer.hpp"

#include <ostream>

namespace aeris {

// Converts a cell type into its display symbol.
namespace {

char symbolFor(CellType type) {
    switch (type) {
        case CellType::Wall:
            return '#';

        case CellType::Floor:
            return '.';

        case CellType::Door:
            return 'D';

        case CellType::Exit:
            return 'E';
    }

    return '?';
}

}

// Displays the entire building grid.
void renderGrid(const BuildingGrid& grid, std::ostream& output) {

    for (std::size_t row = 0; row < grid.rowCount(); ++row) {

        for (std::size_t column = 0;
             column < grid.columnCount();
             ++column) {

            output << symbolFor(grid.at(row, column).type);
        }

        output << '\n';
    }
}

}
