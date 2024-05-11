#include "gtest/gtest.h"
#include "UnloadStation.h"
#include "MiningTruck.h"
#include "Simulation.h"

class UnloadStationTest : public ::testing::Test {};

TEST_F(UnloadStationTest, InitialState) {
    Simulation simulation{0, 0};
    UnloadStation station(simulation);
    EXPECT_EQ(station.getId(), 0);
    EXPECT_EQ(station.getTruckQueueSize(), 0);
}

TEST_F(UnloadStationTest, AddTruckToQueue) {
    Simulation simulation{0, 0};
    UnloadStation station(simulation);
    auto truck = std::make_unique<MiningTruck>(simulation);
    station.addTruckToQueueForUnload(std::move(truck));
    EXPECT_EQ(station.getTruckQueueSize(), 1);
}

TEST_F(UnloadStationTest, RemoveTruckFromQueue) {
    Simulation simulation{0, 0};
    UnloadStation station(simulation);
    auto truck = std::make_unique<MiningTruck>(simulation);
    station.addTruckToQueueForUnload(std::move(truck));
    auto removedTruck = station.removeUnloadedTruckFromQueue();
    EXPECT_NE(removedTruck, nullptr);
    EXPECT_EQ(station.getTruckQueueSize(), 0);
}