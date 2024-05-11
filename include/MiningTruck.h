#pragma once

#include <vector>
#include <random>
class Simulation;

class MiningTruck {
public:
    MiningTruck(Simulation& simulation);
    ~MiningTruck() = default;
    void generateRandomMiningTime();
    int getMiningTime() const;
    void setWaitTime(int unloadingStationWaitTime);
    int getWaitTime() const;
    int getId() const;
    int getTotalMines() const;
    double getAverageMiningTime() const;
    int getTotalUnloads() const;
    double getAverageWaitTime() const;

private:
    int mId;
    int mMiningTime;
    int mUnloadingWaitTime;
    Simulation& mSimulation;
    std::uniform_int_distribution<> mDis;

    // Reporting Stats
    int mTotalMines;
    std::vector<int> mMiningTimes;
    int mTotalUnloads;
    std::vector<int> mWaitTimes;
};