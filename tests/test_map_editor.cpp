
#include "MapEditor.hpp"

#include <iostream>
#include <sstream>
#include <string>

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& name) {
    if (condition) {
        std::cout << "[PASS] " << name << '\n';
        ++passed;
    } else {
        std::cout << "[FAIL] " << name << '\n';
        ++failed;
    }
}

// Test 1: Creating a valid building map.
void testValidMap() {
    std::istringstream input(
        "3\n"
        "4\n"
        "####\n"
        "#D.E\n"
        "####\n"
    );

    std::ostringstream output;

    aeris::BuildingGrid grid =
        aeris::createMapFromInput(input, output);

    check(grid.rowCount() == 3, "Map has correct row count");
    check(grid.columnCount() == 4, "Map has correct column count");

    check(
        grid.at(1, 1).type == aeris::CellType::Door,
        "Door is stored correctly"
    );

    check(
        grid.at(1, 3).type == aeris::CellType::Exit,
        "Exit is stored correctly"
    );
}

// Test 2: Invalid dimensions are rejected.
void testInvalidDimensions() {
    std::istringstream input(
        "2\n"
        "3\n"
        "51\n"
        "4\n"
        "####\n"
        "#..#\n"
        "####\n"
    );

    std::ostringstream output;

    aeris::BuildingGrid grid =
        aeris::createMapFromInput(input, output);

    check(
        grid.rowCount() == 3 && grid.columnCount() == 4,
        "Editor accepts corrected dimensions"
    );

    check(
        output.str().find("Invalid dimension") != std::string::npos,
        "Editor reports invalid dimensions"
    );
}

// Test 3: Invalid rows are rejected.
void testInvalidRows() {
    std::istringstream input(
        "3\n"
        "4\n"
        "###\n"
        "####\n"
        "#X.#\n"
        "#..#\n"
        "####\n"
    );

    std::ostringstream output;

    aeris::BuildingGrid grid =
        aeris::createMapFromInput(input, output);

    check(
        output.str().find("Invalid row length") != std::string::npos,
        "Editor rejects incorrect row length"
    );

    check(
        output.str().find("Invalid symbol") != std::string::npos,
        "Editor rejects invalid symbols"
    );

    check(
        grid.at(1, 1).type == aeris::CellType::Floor,
        "Corrected row is stored successfully"
    );
}

int main() {
    testValidMap();
    testInvalidDimensions();
    testInvalidRows();

    std::cout << "\n========== MAP EDITOR TEST SUMMARY ==========\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "=============================================\n";

    return failed == 0 ? 0 : 1;
}
