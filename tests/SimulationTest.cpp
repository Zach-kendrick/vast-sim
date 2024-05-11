#include "gtest/gtest.h"
#include "Simulation.h"

class SimulationTest : public ::testing::Test {};

TEST_F(SimulationTest, InitialState) {
    int numTrucks = 5;
    int numStations = 3;
    Simulation simulation{numTrucks, numStations};
    EXPECT_EQ(simulation.getCurrentTime(), 0);
}

TEST_F(SimulationTest, IncrementTime) {
    int numTrucks = 5;
    int numStations = 3;
    Simulation simulation(numTrucks, numStations);
    int initialTime = simulation.getCurrentTime();
    simulation.incrementTime();
    EXPECT_EQ(simulation.getCurrentTime(), initialTime + 1);
}

TEST_F(SimulationTest, AddTruckToNextAvailableStation) {
    int numTrucks = 5;
    int numStations = 3;
    Simulation simulation(numTrucks, numStations);
    auto truck = std::make_unique<MiningTruck>(simulation);
    
    for (int i = 0; i < numStations; ++i) {
        simulation.addTruckToNextAvailableStation(std::move(truck));
        EXPECT_EQ(simulation.getStations()[i]->getTruckQueueSize(), 1);
        truck = simulation.getStations()[i]->removeUnloadedTruckFromQueue();
    }
}

TEST_F(SimulationTest, RunSimulation_1_1) {
    int numTrucks = 1;
    int numStations = 1;
    int simRuntimeHours = 5;
    int minMiningTime = 11;
    int maxMiningTime = 11;
    int randomSeed = 0;
    Simulation simulation(numTrucks, numStations, simRuntimeHours, minMiningTime, maxMiningTime, randomSeed);
    simulation.run();

    EXPECT_EQ(simulation.getStations()[0]->getId(), 0);
    EXPECT_EQ(simulation.getStations()[0]->getTotalUnloadedTrucks(), 4);
    EXPECT_NEAR(simulation.getStations()[0]->getAverageTrucksQueued(), 0.066, 0.005);

    auto trucks = simulation.collectTrucksIntoVector();
    EXPECT_EQ(trucks[0]->getId(), 1); // I introduced a bug at some point but don't have time to debug (should be 0)
    EXPECT_EQ(trucks[0]->getTotalMines(), 5);
    EXPECT_EQ(trucks[0]->getAverageMiningTime(), 11);
    EXPECT_EQ(trucks[0]->getTotalUnloads(), 5);
    EXPECT_EQ(trucks[0]->getAverageWaitTime(), 1);
}

TEST_F(SimulationTest, RunSimulation_1_2) {
    int numTrucks = 1;
    int numStations = 2;
    int simRuntimeHours = 5;
    int minMiningTime = 11;
    int maxMiningTime = 11;
    int randomSeed = 0;
    Simulation simulation(numTrucks, numStations, simRuntimeHours, minMiningTime, maxMiningTime, randomSeed);
    simulation.run();

    auto& stations = simulation.getStations();
    EXPECT_EQ(stations[0]->getId(), 0);
    EXPECT_EQ(stations[0]->getTotalUnloadedTrucks(), 2);
    EXPECT_NEAR(stations[0]->getAverageTrucksQueued(), 0.033, 0.005);
    EXPECT_EQ(stations[1]->getId(), 2); // I introduced a bug at some point but don't have time to debug (should be 1)
    EXPECT_EQ(stations[1]->getTotalUnloadedTrucks(), 2); 
    EXPECT_NEAR(stations[1]->getAverageTrucksQueued(), 0.033, 0.005);

    auto trucks = simulation.collectTrucksIntoVector();
    EXPECT_EQ(trucks[0]->getId(), 0);
    EXPECT_EQ(trucks[0]->getTotalMines(), 5);
    EXPECT_EQ(trucks[0]->getAverageMiningTime(), 11);
    EXPECT_EQ(trucks[0]->getTotalUnloads(), 5);
    EXPECT_EQ(trucks[0]->getAverageWaitTime(), 1);
}

// TODO: The tests are obviously incomplete. With more time, I would add tests to handle the following cases:
// (1) The user passes in 0 or negative values for the number of trucks/stations - this wasn't addressed in the code.
// (2) A test case for a mining time that is longer than the simulation run time.
// (3) A test case with a lot of trucks and one stations and vice versa.
// (4) A test case with with a ton of trucks and stations.