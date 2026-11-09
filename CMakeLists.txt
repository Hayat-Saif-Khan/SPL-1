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
    src/MapValidator.cpp
    src/OccupancyGrid.cpp
    src/MapAnalyzer.cpp
    src/OccupancyRenderer.cpp
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

add_executable(test_map_validator
    tests/test_map_validator.cpp
    src/BuildingGrid.cpp
    src/MapValidator.cpp
)

target_include_directories(test_map_validator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(
    NAME MapValidatorTests
    COMMAND test_map_validator
)

add_executable(test_occupancy_grid
    tests/test_occupancy_grid.cpp
    src/BuildingGrid.cpp
    src/OccupancyGrid.cpp
)

target_include_directories(test_occupancy_grid PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(
    NAME OccupancyGridTests
    COMMAND test_occupancy_grid
)

add_executable(test_map_analyzer
    tests/test_map_analyzer.cpp
    src/BuildingGrid.cpp
    src/MapAnalyzer.cpp
)

target_include_directories(test_map_analyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(
    NAME MapAnalyzerTests
    COMMAND test_map_analyzer
)

add_executable(test_occupancy_renderer
    tests/test_occupancy_renderer.cpp
    src/BuildingGrid.cpp
    src/OccupancyGrid.cpp
    src/OccupancyRenderer.cpp
)

target_include_directories(test_occupancy_renderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(
    NAME OccupancyRendererTests
    COMMAND test_occupancy_renderer
)

add_executable(test_agent_model
    tests/test_agent_model.cpp
)

target_include_directories(test_agent_model PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(
    NAME AgentModelTests
    COMMAND test_agent_model
)

endif()