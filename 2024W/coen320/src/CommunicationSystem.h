#ifndef COMMUNICATIONSYSTEM_H
#define COMMUNICATIONSYSTEM_H

#include <atomic>
#include <sys/neutrino.h>
#include <iostream>
#include <mutex>
#include "Timer.h" 
#include "Radar.h"


class CommunicationSystem {
private:
	std::atomic<bool> active;
    int chid; // Channel ID for receiving data from ComputerSystem
    Timer communicationTimer;
    std::mutex mutex; // Mutex for thread safety, if needed

public:
    CommunicationSystem();
    ~CommunicationSystem();
    void start();
    void stop();
    void handleIncomingMessages(); // Processes incoming messages from ComputerSystem
    int getChid() const;

};

#endif // COMMUNICATIONSYSTEM_H
