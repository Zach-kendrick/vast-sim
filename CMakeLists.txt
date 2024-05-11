cmake_minimum_required(VERSION 3.10)
project(LunarMiningSimulation)

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.11.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(include)

add_executable(LunarMiningSimulation
    src/Main.cpp
    src/Simulation.cpp
    src/MiningTruck.cpp
    src/UnloadStation.cpp
)

# Combine all test sources into a single executable
add_executable(LunarMiningSimulationTests
    tests/MiningTruckTest.cpp
    tests/UnloadStationTest.cpp
    tests/SimulationTest.cpp
    src/MiningTruck.cpp
    src/UnloadStation.cpp
    src/Simulation.cpp
)

target_link_libraries(LunarMiningSimulationTests gtest_main)

# Add a single test command for running all tests
add_test(NAME AllTests COMMAND LunarMiningSimulationTests)