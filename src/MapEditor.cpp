
#include "MapEditor.hpp"

#include <charconv>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <system_error>

namespace aeris {

namespace {

constexpr int MIN_DIMENSION = 3;
constexpr int MAX_DIMENSION = 50;

// Checks whether a dimension is within the allowed range.
bool parseDimension(
    const std::string& token,
    std::size_t& dimension
) {
    int value = 0;

    const auto result = std::from_chars(
        token.data(),
        token.data() + token.size(),
        value
    );

    if (result.ec != std::errc{} ||
        result.ptr != token.data() + token.size() ||
        value < MIN_DIMENSION ||
        value > MAX_DIMENSION) {
        return false;
    }

    dimension = static_cast<std::size_t>(value);
    return true;
}

// Reads a valid grid dimension.
std::size_t readDimension(
    std::istream& input,
    std::ostream& output,
    const std::string& label
) {
    std::string token;
    std::size_t dimension = 0;

    while (true) {
        output << label << " (3-50): ";

        if (!(input >> token)) {
            throw std::runtime_error(
                "Input ended while reading grid dimensions."
            );
        }

        if (parseDimension(token, dimension)) {
            return dimension;
        }

        output << "Invalid dimension. Enter an integer from 3 to 50.\n";
    }
}

// Converts a symbol into its corresponding cell type.
bool decodeCellSymbol(char symbol, CellType& type) {

    switch (symbol) {

        case '#':
            type = CellType::Wall;
            return true;

        case '.':
            type = CellType::Floor;
            return true;

        case 'D':
            type = CellType::Door;
            return true;

        case 'E':
            type = CellType::Exit;
            return true;

        default:
            return false;
    }
}

}

// Creates a building grid using user-provided input.
BuildingGrid createMapFromInput(
    std::istream& input,
    std::ostream& output
) {

    output << "\n========== BUILDING MAP EDITOR ==========\n";

    // Read building dimensions.
    const std::size_t rows =
        readDimension(input, output, "Enter number of rows");

    const std::size_t columns =
        readDimension(input, output, "Enter number of columns");

    // Create the grid.
    BuildingGrid grid(rows, columns);

    output << "\nEnter each row using:\n";
    output << "# = Wall\n";
    output << ". = Floor\n";
    output << "D = Door\n";
    output << "E = Exit\n\n";

    // Read and validate every row.
    for (std::size_t row = 0; row < rows; ++row) {

        std::string rowText;

        while (true) {

            output << "Row " << row + 1 << ": ";

            if (!(input >> rowText)) {
                throw std::runtime_error(
                    "Input ended while reading building rows."
                );
            }

            // Check row length.
            if (rowText.size() != columns) {
                output << "Invalid row length. Expected "
                       << columns << " characters.\n";
                continue;
            }

            // Check whether all symbols are valid.
            bool valid = true;

            for (char symbol : rowText) {
                CellType type;

                if (!decodeCellSymbol(symbol, type)) {
                    valid = false;
                    break;
                }
            }

            if (!valid) {
                output << "Invalid symbol. Use only # . D E.\n";
                continue;
            }

            // Store the validated row in the grid.
            for (std::size_t column = 0;
                 column < columns;
                 ++column) {

                CellType type;

                decodeCellSymbol(rowText[column], type);

                grid.at(row, column).type = type;
            }

            break;
        }
    }

    return grid;
}

}
