#ifndef RADAR_H_
#define RADAR_H_

#include "Timer.h"
#include <atomic>
#include <sys/neutrino.h>

struct PositionUpdateMsg {
    int messageType;  // Message type, e.g., position update
    int aircraftID;
    int posX, posY, posZ;
    int velX, velY, velZ;  // Velocity information
};

class Radar {
private:
    std::atomic<bool> active;  // Radar's operational state
    Timer radarTimer;  // Timer for periodic checks
    int chid;  // Channel ID for receiving data
    int computerSystemChid;  // Channel ID for the Computer System

public:
    Radar();
    ~Radar();
    void start();
    void stop();
    void setComputerSystemChid(int chid);
    int getChid() const { return chid; }
    void handleIncomingMessages();  // Processes incoming messages
};

#endif // RADAR_H_
