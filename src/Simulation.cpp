#include "Simulation.h"
#include "MiningTruck.h"
#include "UnloadStation.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>


Simulation::Simulation(int numTrucks, int numStations, int simRuntimeHours, int minMiningTime, int maxMiningTime, std::optional<int> randomSeed)
    : mCurrentTime(0), mCurrentStationIndex(0), mNextTruckId(0), mNextStationId(0), mStations(numStations), kSimRuntimeHours(simRuntimeHours),
    mRandomSeed((randomSeed != std::nullopt) ? randomSeed : std::nullopt), mMinMiningTime(minMiningTime), mMaxMiningTime(maxMiningTime) {

    for (int i = 0; i < numTrucks; ++i) {
        auto truck = std::make_unique<MiningTruck>(*this);
        truck->generateRandomMiningTime();
        mMiningWaitTimes.push(std::move(truck));
    }
    for (int i = 0; i < numStations; ++i) {
        mStations[i] = std::make_unique<UnloadStation>(*this);
    }
}

Simulation::Simulation(int numTrucks, int numStations) : Simulation(numTrucks,  numStations, 72, 12, 60, std::nullopt) {}

void Simulation::run() {

    auto start = std::chrono::steady_clock::now();

    while (mCurrentTime < kSimRuntimeHours * 12) {

        // Remove trucks from stations that are done unloading and send them to mine
        for (auto& station : mStations) {
            // Reporting Stats
            station->updateTrucksQueuedPerTimeStep();

            auto truck = station->removeUnloadedTruckFromQueue();
            if (truck != nullptr) {
                truck->generateRandomMiningTime();
                mMiningWaitTimes.push(std::move(truck));
            }
        }

        // Iterate over the priority queue and remove the trucks that are ready to be unloaded
        while (!mMiningWaitTimes.empty()) {
            const auto& truck = mMiningWaitTimes.top();
            if (truck->getMiningTime() <= mCurrentTime) {
                auto newTruck = std::make_unique<MiningTruck>(*truck);
                mMiningWaitTimes.pop();
                addTruckToNextAvailableStation(std::move(newTruck));
            } else {
                break;
            }
        }

        // Increment current time by 5 minutes
        incrementTime();
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Simulation completed in " << duration << " milliseconds." << std::endl;
}

// TODO: I slapped this together to give me access to all the trucks for the test modules. 
// This is bad design. I should be able to access a reference to all the trucks from some data structure in the simulation class.
// By the way I don't have a linter but I tried my best to keep the formatting consistent.
std::vector<std::unique_ptr<MiningTruck>> Simulation::collectTrucksIntoVector() {
    std::vector<std::unique_ptr<MiningTruck>> mTrucksForReport;

    // Iterate over the trucks in the priority queue
    while (!mMiningWaitTimes.empty()) {
        const auto& truck = mMiningWaitTimes.top();
        mTrucksForReport.emplace_back(std::make_unique<MiningTruck>(*truck));
        mMiningWaitTimes.pop();
    }

    // Iterate over the trucks in the queues of each station
    for (const auto& station : mStations) {
        auto& q = station->getTruckQueue();
        while (!q.empty()) {
            const auto& truck = q.front();
            mTrucksForReport.emplace_back(std::make_unique<MiningTruck>(*truck));
            q.pop();
        }
    }

    // Sort the vector of unique_ptrs by ID (using custom comparison)
    std::sort(mTrucksForReport.begin(), mTrucksForReport.end(),
            [](const std::unique_ptr<MiningTruck>& a, const std::unique_ptr<MiningTruck>& b) {
                return a.get()->getId() < b.get()->getId();
            });

    return mTrucksForReport;
}

int Simulation::getSimRuntimeHours() const {
    return kSimRuntimeHours;
}

int Simulation::getCurrentTime() const {
    return mCurrentTime;
}

void Simulation::incrementTime() {
    mCurrentTime++;
}

void Simulation::addTruckToNextAvailableStation(std::unique_ptr<MiningTruck> truck) {
    auto& station = *mStations[mCurrentStationIndex];
    mCurrentStationIndex = (mCurrentStationIndex + 1) % mStations.size();
    station.addTruckToQueueForUnload(std::move(truck));
}

int Simulation::getNextTruckId() {
    return mNextTruckId++;
}

int Simulation::getNextStationId() {
    return mNextStationId++;
}

const std::vector<std::unique_ptr<UnloadStation>>& Simulation::getStations() const {
    return mStations;
}

const std::optional<int> Simulation::getRandomSeed() {
    if (mRandomSeed.has_value()) {
       (*mRandomSeed)++;
    }
    return mRandomSeed;
}

const int Simulation::getMinMiningTime() const {
    return mMinMiningTime;
}

const int Simulation::getMaxMiningTime() const {
    return mMaxMiningTime;
}

// TODO: Saving data to a text file is a bad idea. Should be saved to a CSV file for easy post processing scripts.
// I don't like some of the metrics I computed in retrospect. Would probably want to rethink what is important to measure here.
void Simulation::generateReport() {
    std::ofstream file("../data/simulation_report.txt");
    if (file.is_open()) {
        file << "Truck Report:\n";
        file << "Truck ID\tTotal Mines\tAvg Mining Time\tTotal Unloads\tAvg Wait Time\n";
        
        // Iterate over the trucks in the priority queue
        while (!mMiningWaitTimes.empty()) {
            const auto& truck = mMiningWaitTimes.top();
            file << truck->getId() << "\t\t\t\t"
                 << truck->getTotalMines() << "\t\t\t\t"
                 << truck->getAverageMiningTime() << "\t\t\t\t"
                 << truck->getTotalUnloads() << "\t\t\t\t"
                 << truck->getAverageWaitTime() << "\n";
            mMiningWaitTimes.pop();
        }
        
        // Iterate over the trucks in the queues of each station
        for (const auto& station : mStations) {
            auto& q = station->getTruckQueue();
            while (!q.empty()) {
                const auto& truck = q.front();
                file << truck->getId() << "\t\t\t\t" << truck->getTotalMines() << "\t\t\t\t"
                     << truck->getAverageMiningTime() << "\t\t\t\t" << truck->getTotalUnloads()
                     << "\t\t\t\t" << truck->getAverageWaitTime() << "\n";
                q.pop();
            }
        }
        
        file << "\nUnload Station Report:\n";
        file << "Station ID\tTotal Unloaded\tAvg Trucks Queued\n";
        for (const auto& station : mStations) {
            file << station->getId() << "\t\t\t\t"
                 << station->getTotalUnloadedTrucks() << "\t\t\t\t"
                 << station->getAverageTrucksQueued() << "\n";
        }
        
        file.close();
        std::cout << "Simulation report generated: simulation_report.txt" << std::endl;
    } else {
        std::cout << "Unable to generate simulation report." << std::endl;
    }
}