#include <iostream>
#include "Simulation.h"

int main() {
    // TODO: It might be better to have these as command line arguements.
    int numTrucks, numStations;
    std::cout << "Enter the number of mining trucks: ";
    std::cin >> numTrucks;
    std::cout << "Enter the number of unload stations: ";
    std::cin >> numStations;

    Simulation simulation(numTrucks, numStations);
    simulation.run();
    simulation.generateReport();

    return 0;
}