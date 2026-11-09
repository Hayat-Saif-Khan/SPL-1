
#include "MapAnalyzer.hpp"

namespace aeris {

MapStatistics analyzeMap(const BuildingGrid& grid) {

    MapStatistics statistics;

    const std::size_t rows = grid.rowCount();
    const std::size_t columns = grid.columnCount();

    statistics.totalCells = rows * columns;

    // Examine every cell in the building.

    for (std::size_t row = 0; row < rows; ++row) {

        for (std::size_t column = 0;
             column < columns;
             ++column) {

            const CellType type = grid.at(row, column).type;

            switch (type) {

                case CellType::Wall:
                    ++statistics.wallCount;
                    break;

                case CellType::Floor:
                    ++statistics.floorCount;
                    break;

                case CellType::Door:
                    ++statistics.doorCount;
                    break;

                case CellType::Exit:
                    ++statistics.exitCount;
                    break;
            }
        }
    }

    // Calculate total walkable cells.

    statistics.walkableCount =
        statistics.floorCount +
        statistics.doorCount +
        statistics.exitCount;

    // Calculate the percentage of blocked cells.
    
    if (statistics.totalCells > 0) {

        statistics.blockedPercentage =
            (static_cast<double>(statistics.wallCount) /
             static_cast<double>(statistics.totalCells)) * 100.0;
    }

    return statistics;
}

}
