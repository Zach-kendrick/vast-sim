#pragma once

#include <vector>
#include <queue>
#include <optional>

#include "MiningTruck.h"
#include "UnloadStation.h"

struct CompareTruckTimes {
    bool operator()(const std::unique_ptr<MiningTruck>& a, const std::unique_ptr<MiningTruck>& b) {
        return a->getMiningTime() > b->getMiningTime();
    }
};

class Simulation {
public:
    Simulation(int numTrucks, int numStations, int simRuntimeHours, int minMiningTime, int maxMiningTime, std::optional<int> randomSeed);
    Simulation(int numTrucks, int numStations);
    ~Simulation() = default;
    void run();
    int getSimRuntimeHours() const;
    int getCurrentTime() const;
    void incrementTime();
    int getNextTruckId();
    int getNextStationId();
    const std::vector<std::unique_ptr<UnloadStation>>& getStations() const;
    void addTruckToNextAvailableStation(std::unique_ptr<MiningTruck> truck);
    void generateReport();

    const std::optional<int> getRandomSeed();
    const int getMinMiningTime() const;
    const int getMaxMiningTime() const;

    // Reporting Stats
    std::vector<std::unique_ptr<MiningTruck>> collectTrucksIntoVector(); 

private:
    // TODO: this assumes increments of 5 minutes. This is a very bad design but it gets the job done.
    int mCurrentTime;

    int mNextTruckId;
    int mNextStationId;
    std::vector<std::unique_ptr<UnloadStation>> mStations;
    int mCurrentStationIndex;
    std::priority_queue<std::unique_ptr<MiningTruck>, std::vector<std::unique_ptr<MiningTruck>>, CompareTruckTimes> mMiningWaitTimes;

    // TODO: I would likely have these params ingested from a config file
    const int kSimRuntimeHours;
    std::optional<int> mRandomSeed;
    int mMinMiningTime;
    int mMaxMiningTime;   
};