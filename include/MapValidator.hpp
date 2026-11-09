
#pragma once

#include "BuildingGrid.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace aeris {

struct MapValidationResult {

    std::vector<std::string> errors;

    std::size_t unreachableCellCount{0};

    bool isValid() const noexcept {
        return errors.empty();
    }
};

// Checks whether a building map is suitable for evacuation.
MapValidationResult validateMap(const BuildingGrid& grid);

}
