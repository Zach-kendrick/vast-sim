#include "gtest/gtest.h"
#include "MiningTruck.h"
#include "Simulation.h"

class MiningTruckTest : public ::testing::Test {};

TEST_F(MiningTruckTest, InitialState) {
    Simulation simulation{0, 0};
    MiningTruck truck(simulation);
    EXPECT_EQ(truck.getId(), 0);
    EXPECT_EQ(truck.getMiningTime(), 0);
    EXPECT_EQ(truck.getWaitTime(), 0);
}

TEST_F(MiningTruckTest, GenerateRandomMiningTime) {
    Simulation simulation{0, 0};
    MiningTruck truck(simulation);
    truck.generateRandomMiningTime();
    int time = truck.getMiningTime();
    EXPECT_TRUE(time >= 12 && time <= 60);
}

TEST_F(MiningTruckTest, SetWaitTime) {
    Simulation simulation{0, 0};
    MiningTruck truck(simulation);
    simulation.incrementTime();
    truck.setWaitTime(10);
    EXPECT_EQ(truck.getWaitTime(), 11);
}