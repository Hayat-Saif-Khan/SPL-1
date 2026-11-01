# AERIS
## Adaptive Emergency & Incident Response Simulator

AERIS is a CLI-based emergency evacuation simulator developed as the SPL-1 project at the Institute of Information Technology (IIT), University of Dhaka.

The project aims to simulate how multiple agents evacuate a building during emergency situations involving dynamic hazards such as fire, smoke, and blocked routes.

## Planned Features

- Building map creation through a CLI-based manual map builder.
- Conversion of building layouts into grid and graph representations.
- Simulation of fire, smoke, and blocked routes.
- Multiple evacuation agents with different characteristics.
- Pathfinding using BFS, Dijkstra, and A*.
- Dynamic route replanning as hazards change.
- Step-by-step simulation animation.
- Evacuation analysis, including evacuation time and bottlenecks.
- Explainable route decisions for individual agents.

## Technology Stack

- Language: C++17
- Build system: CMake
- Development environment: Ubuntu on WSL

## Project Structure

- `src/` — C++ source files
- `include/` — Header files
- `data/` — Maps and simulation data
- `tests/` — Tests
- `docs/` — Documentation

## Project Status

In development. Features listed above are planned and will be implemented incrementally.

---

SPL-1 Project | IIT, University of Dhaka
