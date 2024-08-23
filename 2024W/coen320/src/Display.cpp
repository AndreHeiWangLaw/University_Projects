#include "Display.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include "OperatorConsole.h"


// Assume GRID_SIZE is the dimension of the grid (e.g., 100x100)
const int GRID_SIZE = 50;
const int SCALE_FACTOR = 2000;

Display::Display() : active(false), chid(-1) {
    chid = ChannelCreate(0);
    if (chid == -1) {
        std::cerr << "ChannelCreate failed: " << strerror(errno) << std::endl;
    }
}

Display::~Display() {
    stop();
    if (chid != -1) {
        ChannelDestroy(chid);
    }
}

void Display::start() {
    if (!active.load()) {
        active = true;
        try {
            displayTimer.startTimer(1000000000LL, [this]() {
                this->handleIncomingMessages();
            });
            xyDisplayTimer.startTimer(5000000000LL, [this]() {
                this->updateXYPlaneView();
            });
        } catch (const std::runtime_error& e) {
            std::cerr << "Timer error: " << e.what() << std::endl;
        }
    }
}


void Display::stop() {
    if (active.load()) {
        active = false;
        displayTimer.stopTimer();
    }
}

void Display::handleIncomingMessages() {
    if (!active) return;

    struct _msg_info msgInfo;
    int rcvid;
    PositionUpdateMsg msg;

    while ((rcvid = MsgReceive(chid, &msg, sizeof(msg), &msgInfo)) != -1) {
    	std::lock_guard<std::mutex> guard(OperatorConsole::getPrintMutex());
        std::cout << "Radar Detected Aircraft ID " << msg.aircraftID
                  << ", at X: " << msg.posX
                  << ", Y: " << msg.posY
                  << ", Z: " << msg.posZ
				  << " within the En-Route boundaries" << std::endl;
        std::cout << "---------------------------------------------------------" << std::endl;

        // Assuming latestPositions is a std::map<int, AircraftPosition>
        AircraftPosition& position = latestPositions[msg.aircraftID];
        position.id = msg.aircraftID;
        position.posX = msg.posX;
        position.posY = msg.posY;
        position.posZ = msg.posZ;
        position.lastUpdate = std::chrono::steady_clock::now();


        MsgReply(rcvid, EOK, NULL, 0);
    }


    if (rcvid == -1 && errno != EAGAIN) {
        std::cerr << "MsgReceive failed: " << strerror(errno) << '\n';
    }
}

int Display::getChid() const {
    return chid;
}

void Display::updateXYPlaneView() {
	auto now = std::chrono::steady_clock::now();
	const std::chrono::seconds timeout(0);
	std::lock_guard<std::mutex> guard(OperatorConsole::getPrintMutex());
    // Initialize a 2D vector to represent the grid
    std::vector<std::vector<std::set<int>>> grid(GRID_SIZE, std::vector<std::set<int>>(GRID_SIZE));

    // Populate the grid with aircraft IDs, scaled down by the SCALE_FACTOR
    for (const auto& [id, pos] : latestPositions) {
        // Scale positions to grid coordinates
        int gridX = pos.posX / SCALE_FACTOR;
        int gridY = pos.posY / SCALE_FACTOR;

        // Check scaled positions are within the grid bounds and not negative
        if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
            grid[gridX][gridY].insert(id);
        }
    }
    for (auto it = latestPositions.begin(); it != latestPositions.end(); ) {
            if (now - it->second.lastUpdate > timeout) {
                it = latestPositions.erase(it);
            } else {
                ++it;
            }
        }

    printXYPlaneView(grid); // Call the method to print the grid
}


void Display::printXYPlaneView(const std::vector<std::vector<std::set<int>>>& grid) {
    std::cout << "\nXY Plane View (Grid " << GRID_SIZE << "x" << GRID_SIZE << ", each cell represents approximately " << SCALE_FACTOR << "x" << SCALE_FACTOR << " feet):\n\n";

    std::stringstream detailedInfo; // Use a stringstream to collect detailed information for printing later

    // Iterate through the grid to print the compact representation
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            if (!grid[x][y].empty()) {
                std::cout << "|"; // Presence of aircraft marker
                if (grid[x][y].size() > 1) {
                    std::cout << "*"; // Multiple aircraft indicator
                    // Calculate actual coordinates from cell numbers
                    int actualX = x * SCALE_FACTOR;
                    int actualY = y * SCALE_FACTOR;
                    // Collect detailed information for aircraft in this cell
                    detailedInfo << "Coordinates (" << actualX << ", " << actualY << ") contains aircraft IDs - ";
                    for (const auto& id : grid[x][y]) {
                        detailedInfo << "[" << id << "]";
                    }
                    detailedInfo << "***\n\n\n";
                } else {
                    std::cout << *(grid[x][y].begin()); // Single aircraft ID
                }
            } else {
                std::cout << " ."; // Empty cell indicator
            }
        }
        std::cout << "|\n"; // End of row
    }

    // Now print the detailed information collected
    if (!detailedInfo.str().empty()) {
        std::cout << "\n\n***Detailed Aircraft Information for Cells with Multiple Aircraft\n";
        std::cout << detailedInfo.str();
    } else {
        std::cout << "\n\n***No cells with multiple aircraft.***\n\n";
    }

    std::cout << std::flush; // Ensure output is displayed immediately
}
