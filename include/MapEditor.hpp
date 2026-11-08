#pragma once

#include "BuildingGrid.hpp"

#include <iosfwd>

namespace aeris {

BuildingGrid createMapFromInput(
    std::istream& input,
    std::ostream& output
);

}