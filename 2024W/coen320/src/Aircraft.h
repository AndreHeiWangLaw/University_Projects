#ifndef AIRCRAFT_H_
#define AIRCRAFT_H_

#include <thread>
#include <atomic>
#include "Timer.h"

class Aircraft {
private:
    int id;
    double posX, posY, posZ; // Position
    double velX, velY, velZ; // Velocity
    std::atomic<bool> active; // To keep track of the aircraft's activity
    std::thread worker; // Worker thread for the aircraft
    Timer positionUpdateTimer; // Timer for periodic position updates
    int radarChid; // Channel ID for the Radar

    void updatePosition();
    void sendPositionUpdate();

public:
    Aircraft(int id, double posX, double posY, double posZ, double velX, double velY, double velZ, int radarChid);
    ~Aircraft();

    void start();
    void stop();
    static void taskFunction(Aircraft* aircraft); // Static to be used with thread
};

#endif // AIRCRAFT_H_
