#include "Simulation.h"
#include "MiningTruck.h"
#include <chrono>
#include <thread>
#include <numeric>
#include <iostream>

MiningTruck::MiningTruck(Simulation& simulation) : 
mId(simulation.getNextTruckId()), mMiningTime(0), mUnloadingWaitTime(0), mSimulation(simulation), 
mDis(simulation.getMinMiningTime(), simulation.getMaxMiningTime()), 
mTotalMines(0), mMiningTimes(0, 0), mTotalUnloads(0),  mWaitTimes(0,0) {}

void MiningTruck::generateRandomMiningTime() {
    std::random_device rd;
    auto seed = mSimulation.getRandomSeed();
    std::mt19937 gen((seed != std::nullopt) ? seed.value() : rd());
    int randomMiningTime = mDis(gen);
    mMiningTime = randomMiningTime + mSimulation.getCurrentTime();

    // Reporting Stats
    mMiningTimes.push_back(randomMiningTime);
    mTotalMines++;
}

int MiningTruck::getMiningTime() const {
    return mMiningTime;
}

void MiningTruck::setWaitTime(int unloadingStationWaitTime) {
    mUnloadingWaitTime = unloadingStationWaitTime + mSimulation.getCurrentTime();

    // Reporting Stats
    mWaitTimes.push_back(unloadingStationWaitTime);
    mTotalUnloads++;
}

int MiningTruck::getWaitTime() const {
    return mUnloadingWaitTime;
}

int MiningTruck::getId() const {
    return mId;
}

// Reporting Stats
int MiningTruck::getTotalMines() const { 
    return mTotalMines; 
}
double MiningTruck::getAverageMiningTime() const {
    if (mMiningTimes.empty()) { 
        return 0.0;
    }
    return std::accumulate(mMiningTimes.begin(), mMiningTimes.end(), 0.0) / mMiningTimes.size();
}

int MiningTruck::getTotalUnloads() const { 
    return mTotalUnloads; 
}

double MiningTruck::getAverageWaitTime() const {
    if (mWaitTimes.empty()) {
        return 0.0;
    }
    return std::accumulate(mWaitTimes.begin(), mWaitTimes.end(), 0.0) / mWaitTimes.size();
}