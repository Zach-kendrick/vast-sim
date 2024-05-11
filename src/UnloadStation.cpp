#include "Simulation.h"
#include "UnloadStation.h"
#include "MiningTruck.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <numeric>

UnloadStation::UnloadStation(Simulation& simulation) : mTotalWaitTime(0), mId(simulation.getNextStationId()), mTotalUnloadedTrucks(0), 
mTrucksQueuedPerTimeStep(simulation.getSimRuntimeHours() * 12, 0), mTrucksQueuedPerTimeStepIdx(0), mSimulation(simulation) {}

std::unique_ptr<MiningTruck> UnloadStation::removeUnloadedTruckFromQueue() {
    if (!mTruckQueue.empty()) {
        std::unique_ptr<MiningTruck> truck = std::move(mTruckQueue.front());
        mTruckQueue.pop();
        mTotalWaitTime--;

        // Reporting Stats
        mTotalUnloadedTrucks++;

        return truck;
    }
    return nullptr;
}

void UnloadStation::addTruckToQueueForUnload(std::unique_ptr<MiningTruck> truck) {
    mTotalWaitTime++;
    truck->setWaitTime(mTotalWaitTime);
    mTruckQueue.push(std::move(truck));

    // Reporting Stats
    mTrucksQueuedPerTimeStep[mTrucksQueuedPerTimeStepIdx]++;
}

int UnloadStation::getId() const {
    return mId;
}

int UnloadStation::getTruckQueueSize() const {
    return mTruckQueue.size();
}

std::queue<std::unique_ptr<MiningTruck>>& UnloadStation::getTruckQueue() {
    return mTruckQueue;
}

std::vector<std::unique_ptr<MiningTruck>> UnloadStation::getTrucksInQueue() {
    std::vector<std::unique_ptr<MiningTruck>> trucks(mTruckQueue.size());

    while (!mTruckQueue.empty()) {
        trucks.emplace_back(std::move(mTruckQueue.front()));
        mTruckQueue.pop();
    }
    return trucks;
}

// Reporting Stats
void UnloadStation::updateTrucksQueuedPerTimeStep() {
    mTrucksQueuedPerTimeStepIdx++;
}

int UnloadStation::getTotalUnloadedTrucks() const {
    return mTotalUnloadedTrucks; 
    }
double UnloadStation::getAverageTrucksQueued() const {
    if (mTrucksQueuedPerTimeStep.empty()) 
        return 0.0;
    return std::accumulate(mTrucksQueuedPerTimeStep.begin(), mTrucksQueuedPerTimeStep.end(), 0.0) / mTrucksQueuedPerTimeStep.size();
}