#ifndef COMPUTERSYSTEM_H_
#define COMPUTERSYSTEM_H_

#include <atomic>
#include <sys/neutrino.h>
#include "Timer.h"  // Reuse the Timer for periodic checks
#include <map>
#include <vector>
#include <cmath>

// Reuse the PositionUpdateMsg structure
#include "Radar.h"

struct AircraftState {
    int id;
    int posX, posY, posZ;
    int velX, velY, velZ;

    // Predicts the position after a certain time interval (in seconds)
    AircraftState predictPosition(int seconds) const {
        return {id,
                posX + velX * seconds,
                posY + velY * seconds,
                posZ + velZ * seconds,
                velX, velY, velZ};
    }
};

struct ViolationMsg {
    int messageType;  // A specific identifier for violation messages
    int aircraftID1;
    int aircraftID2;
    int posX1, posY1, posZ1; // Position of the first aircraft
    int posX2, posY2, posZ2; // Position of the second aircraft
};


class ComputerSystem {
private:
    std::atomic<bool> active;  // Computer System's operational state
    Timer processTimer, violationCheckTimer;  // Timer for periodic processing
    int chid;  // Channel ID for receiving data from Radar
    int displayChid;  // Channel ID for the Display System

    int operatorConsoleChid; // Channel ID for the Operator System
    int communicationSystemChid; // Channel ID for the Communication System
    Timer logTimer;
    std::map<int, AircraftState> aircraftStates; // Tracks state of each aircraft
    std::mutex stateMutex; // Protects access to aircraftStates

    void checkForViolations(); // Method to check for separation violations

public:
    ComputerSystem();
    ~ComputerSystem();
    void start();
    void stop();
    void setDisplayChid(int chid);
    void setOperatorConsoleChid(int chid);
    void setCommunicationSystemChid(int chid);
    void logAirspace();
    void handleIncomingMessages();  // Processes incoming messages from Radar
    int getChid() const { return chid; }
};

#endif // COMPUTERSYSTEM_H_
