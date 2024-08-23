#include "InputStrings.h" // Ensure this is included at the top of your file
#include "Aircraft.h"
#include "Radar.h"
#include "ComputerSystem.h"
#include "Display.h"
#include "OperatorConsole.h"
#include "CommunicationSystem.h"
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int main() {
    Radar radar;
    ComputerSystem computerSystem;
    Display display;
    OperatorConsole operatorConsole;

    CommunicationSystem communicationSystem;

    computerSystem.setDisplayChid(display.getChid());
    computerSystem.setCommunicationSystemChid(communicationSystem.getChid());
    radar.setComputerSystemChid(computerSystem.getChid());
    computerSystem.setOperatorConsoleChid(operatorConsole.getChid());

    radar.start();
    computerSystem.start();
    display.start();
    operatorConsole.start();
    communicationSystem.start();

    std::istringstream inFile(MED_LOAD); // Use istringstream to read from LOW_LOAD string
    std::string line;
    std::vector<Aircraft*> aircrafts;

    while (getline(inFile, line)) {
        std::istringstream iss(line);
        int id, posX, posY, posZ, velX, velY, velZ;
        if (!(iss >> id >> posX >> posY >> posZ >> velX >> velY >> velZ)) {
            std::cerr << "Error reading line: " << line << "\n";
            break;
        }

        Aircraft* aircraft = new Aircraft(id, posX, posY, posZ, velX, velY, velZ, radar.getChid());
        aircrafts.push_back(aircraft);
        aircraft->start();
    }

    std::cout << "Simulation running. Please wait...\n";
    sleep(120); // Adjust as necessary for your simulation

    for (auto* aircraft : aircrafts) {
        aircraft->stop();
        delete aircraft;
    }
    aircrafts.clear();

    radar.stop();
    computerSystem.stop();
    display.stop();
    operatorConsole.stop();
    communicationSystem.stop();

    return 0;
}
