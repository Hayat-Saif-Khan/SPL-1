# AERIS
## Adaptive Emergency & Incident Response Simulator

AERIS is a CLI-based emergency evacuation simulator developed as the SPL-1 project at the Institute of Information Technology (IIT), University of Dhaka.

The project aims to simulate how multiple agents evacuate a building during emergency situations involving dynamic hazards such as fire, smoke, and blocked routes.

## Implemented Features

### Building Map Management
- Manual rectangular building map creation through an interactive CLI.
- Two-dimensional building grid representation.
- Support for four cell types: Wall, Floor, Door, and Exit.
- Terminal-based building map visualization.

### Map Validation and Analysis
- Multi-source Breadth-First Search (BFS) for building map validation.
- Detection of disconnected and unreachable walkable areas.
- Validation of evacuation exit availability.
- Building map statistics and analysis, including:
  - Total number of cells.
  - Wall, floor, door, and exit counts.
  - Total walkable cells.
  - Blocked-cell percentage.

### Occupancy Grid
- Automatic conversion of building layouts into occupancy grids.
- Classification of cells as Blocked or Free.
- Terminal-based occupancy grid visualization.

### Testing
- Automated unit and integration testing using CTest.
- Tests for building grids, map editing, map validation, occupancy grids, map analysis, and occupancy visualization.

## Technology Stack

- **Language:** C++17
- **Build System:** CMake
- **Testing:** CTest
- **Development Environment:** Ubuntu on WSL
- **Version Control:** Git and GitHub

## Project Structure

- `src/` — C++ source files and module implementations.
- `include/` — Header files and module interfaces.
- `data/` — Building maps and simulation data.
- `tests/` — Automated test files.
- `docs/` — Project documentation.
- `CMakeLists.txt` — CMake build configuration.

## Current Limitations

- Supports single-floor building layouts.
- Uses a rectangular grid representation.
- Building maps are currently entered manually through the CLI.
- Map persistence and file loading are not implemented yet.
- Fire, smoke, and dynamic hazards are not implemented yet.
- Evacuation agents and pathfinding are not implemented yet.
- Evacuation simulation and dynamic route replanning are future development tasks.

## Planned Features

- Building map saving and loading.
- Multiple-floor building layouts and staircases.
- Dynamic simulation of fire, smoke, and blocked routes.
- Multiple evacuation agents with different characteristics.
- Pathfinding using BFS, Dijkstra, and A*.
- Dynamic route replanning as hazards change.
- Step-by-step evacuation simulation animation.
- Evacuation analysis, including evacuation time and bottlenecks.
- Explainable route decisions for individual agents.

## Project Status

In development. The building map creation, validation, occupancy grid, and map analysis foundation have been implemented. The remaining simulation features will be developed incrementally.

---

**SPL-1 Project | IIT, Institute of Information Technology, University of Dhaka**
