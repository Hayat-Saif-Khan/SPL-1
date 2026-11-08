cmake_minimum_required(VERSION 3.20)

project(AERIS
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(aeris
    src/main.cpp
    src/BuildingGrid.cpp
    src/GridRenderer.cpp
    src/MapEditor.cpp
)

target_include_directories(aeris PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(CTest)

if(BUILD_TESTING)
    add_executable(test_building_grid
        tests/test_building_grid.cpp
        src/BuildingGrid.cpp
        src/GridRenderer.cpp
    )

    target_include_directories(test_building_grid PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    add_test(
        NAME BuildingGridTests
        COMMAND test_building_grid
    )

    add_executable(test_map_editor
    tests/test_map_editor.cpp
    src/BuildingGrid.cpp
    src/MapEditor.cpp
)

target_include_directories(test_map_editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(
    NAME MapEditorTests
    COMMAND test_map_editor
)

endif()