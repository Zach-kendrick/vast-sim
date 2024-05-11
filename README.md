# Lunar Mining Simulation

The Lunar Mining Simulation is a C++ project that simulates a lunar Helium-3 space mining operation. The simulation manages and tracks the efficiency of mining trucks and unload stations over a continuous 72-hour operation.

## Table of Contents

- [Introduction](#introduction)
- [Codebase Structure](#codebase-structure)
- [Implementation Details](#implementation-details)
- [Testing](#testing)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [Dependencies](#dependencies)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The Lunar Mining Simulation project aims to provide a realistic simulation of a lunar Helium-3 mining operation. It allows users to configure the number of mining trucks and unload stations and generates a report with statistics for each component.

## Codebase Structure

The codebase is organized into the following directories:

- `include/`: Contains the header files for the project.
- `src/`: Contains the source files for the project.
- `tests/`: Contains the unit test files for the project.

### Header Files

- `Simulation.h`: Defines the `Simulation` class, which manages the overall simulation.
- `MiningTruck.h`: Defines the `MiningTruck` class, representing a mining truck in the simulation.
- `UnloadStation.h`: Defines the `UnloadStation` class, representing an unload station in the simulation.

### Source Files

- `Main.cpp`: Contains the main entry point of the program and initializes the simulation.
- `Simulation.cpp`: Implements the functionality of the `Simulation` class.
- `MiningTruck.cpp`: Implements the functionality of the `MiningTruck` class.
- `UnloadStation.cpp`: Implements the functionality of the `UnloadStation` class.

### Test Files

- `MiningTruckTest.cpp`: Contains unit tests for the `MiningTruck` class.
- `UnloadStationTest.cpp`: Contains unit tests for the `UnloadStation` class.
- `SimulationTest.cpp`: Contains unit tests for the `Simulation` class.

## Implementation Details

The simulation flow is as follows:

1. The simulation starts by creating mining trucks and unload stations based on the provided configuration.
2. The `run()` method of the `Simulation` class is called to start the simulation.
3. In each iteration of the simulation loop:
   - Trucks that have finished unloading are removed from the unloading station queue and sent back to mine with a random mining time. Note, I assume that the transportation time is computed as part of the random mining time.
   - Trucks that have completed mining are added to the next available unload station. This is done in a cyclic manner since the unloading time is constant. This provides with load balanced station queues.
   - The current time is incremented. Each time increment is a unit of 5 minutes since this is the minimum amount of time in the simulation.
4. The simulation continues until the specified duration is reached.
5. After the simulation ends, statistics are collected and reported for each mining truck and unload station.

## Testing

The project uses the Google Test framework for unit testing. The test files are located in the `tests/` directory. Each test file focuses on testing a specific class (`MiningTruck`, `UnloadStation`, or `Simulation`).

To run the tests, build the project using CMake and execute the `LunarMiningSimulationTests` executable.

## Build Instructions

1. Clone the repository.
2. Create a build directory: `mkdir build && cd build`
3. Generate the build files: `cmake ..`
4. Build the project: `make -j`

## Usage

1. Build the project using the provided CMake configuration.
2. Run the simulation: `./LunarMiningSimulation`
3. The simulation will execute and generate a report with the statistics for each mining truck and unload station in the `data/` directory.

## Dependencies

- C++17
- CMake (version 3.10 or higher)
- Google Test (fetched automatically via CMake)

## Contributing

Contributions to the Lunar Mining Simulation project are welcome! If you find any issues or have suggestions for improvements, please submit an issue or a pull request on the project's GitHub repository.

## License

The Lunar Mining Simulation project is open-source and available under the [MIT License](LICENSE).