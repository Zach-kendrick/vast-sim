#pragma once

class Simulation;
#include <queue>

class MiningTruck;

class UnloadStation {
public:
    UnloadStation(Simulation& simulation);
    ~UnloadStation() = default;
    std::unique_ptr<MiningTruck> removeUnloadedTruckFromQueue();
    void addTruckToQueueForUnload(std::unique_ptr<MiningTruck> truck);
    int getId() const;
    int getTruckQueueSize() const;
    std::queue<std::unique_ptr<MiningTruck>>& getTruckQueue();
    std::vector<std::unique_ptr<MiningTruck>> getTrucksInQueue();

    // Reporting Stats
    void updateTrucksQueuedPerTimeStep();
    int getTotalUnloadedTrucks() const;
    double getAverageTrucksQueued() const;


private:
    int mId;
    int mTotalWaitTime;
    std::queue<std::unique_ptr<MiningTruck>> mTruckQueue;
    Simulation& mSimulation;

    // Reporting Stats
    int mTotalUnloadedTrucks;
    std::vector<int> mTrucksQueuedPerTimeStep;
    int mTrucksQueuedPerTimeStepIdx;
};