#pragma once

#include "BuildingGrid.hpp"

#include <iosfwd>

namespace aeris {

void renderGrid(const BuildingGrid& grid, std::ostream& output);

}