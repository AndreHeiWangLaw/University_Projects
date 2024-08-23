#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <atomic>
#include <sys/neutrino.h>
#include "Timer.h"
#include "Radar.h"
#include <vector>
#include <mutex>
#include <map>
#include <string>
#include <set>
#include <chrono>

struct AircraftPosition {
    int id;
    int posX, posY, posZ;
    std::chrono::steady_clock::time_point lastUpdate; // Timestamp of the last update
};


class Display {
private:
    std::atomic<bool> active;
    Timer displayTimer, xyDisplayTimer; // Add a second timer for the XY plane display task
    int chid;
    std::mutex printMutex;
    std::map<int, AircraftPosition> latestPositions; // Stores the latest position for each aircraft
    std::map<std::pair<int, int>, std::vector<int>> xyPlaneView; // Maps XY coordinates to aircraft IDs

public:
    Display();
    ~Display();
    void start();
    void stop();
    void handleIncomingMessages();
    void updateXYPlaneView();
    void printXYPlaneView();
    void printXYPlaneView(const std::vector<std::vector<std::set<int>>>& grid);
    int getChid() const;
};


#endif // DISPLAY_H_
