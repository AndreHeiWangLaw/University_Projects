#include "ComputerSystem.h"
#include <iostream>
#include <cstring>
#include <sys/netmgr.h>
#include <cmath>
#include <unistd.h>

#include <fcntl.h>

ComputerSystem::ComputerSystem() : active(false), chid(-1), displayChid(-1), operatorConsoleChid(-1), communicationSystemChid(-1) {
    // Create a channel for Radar to send updates
    chid = ChannelCreate(0);
    if (chid == -1) {
        std::cerr << "ChannelCreate failed: " << strerror(errno) << std::endl;
    }
}

ComputerSystem::~ComputerSystem() {
    stop();
    if (chid != -1) {
        ChannelDestroy(chid);
    }
}

void ComputerSystem::start() {
    if (!active.load()) {
        active = true;
        processTimer.startTimer(1000000000LL, [this]() { this->handleIncomingMessages(); });
        violationCheckTimer.startTimer(3000000000LL, [this]() { this->checkForViolations(); }); // Check every 3 seconds
        logTimer.startTimer(5000000000LL, [this]() { this->logAirspace(); }); // per 5 seconds
    }
}


void ComputerSystem::stop() {
    if (active.load()) {
        active = false;
        processTimer.stopTimer();
        logTimer.stopTimer();
    }
}

void ComputerSystem::setDisplayChid(int chid) {
    this->displayChid = chid;
}

void ComputerSystem::setOperatorConsoleChid(int chid) {
    this->operatorConsoleChid = chid;
}

void ComputerSystem::setCommunicationSystemChid(int chid) {
    this->communicationSystemChid = chid;
}

void ComputerSystem::handleIncomingMessages() {
    if (!active.load()) return;

    struct _msg_info msgInfo;
    int rcvid;
    PositionUpdateMsg msg;

    while ((rcvid = MsgReceive(chid, &msg, sizeof(msg), &msgInfo)) != -1) {
        // Update the aircraft state with the received message
        std::lock_guard<std::mutex> guard(stateMutex); // Protect shared state access
        aircraftStates[msg.aircraftID] = {msg.aircraftID, msg.posX, msg.posY, msg.posZ, msg.velX, msg.velY, msg.velZ};

        // Forward the message to the Display System
        int coid = ConnectAttach(ND_LOCAL_NODE, 0, displayChid, _NTO_SIDE_CHANNEL, 0);
        if (coid != -1) {
            MsgSend(coid, &msg, sizeof(msg), NULL, NULL);
            ConnectDetach(coid);
        } else {
            std::cerr << "Failed to send data to Display System." << std::endl;
        }

        // Forward the message to the Communication System
        coid = ConnectAttach(ND_LOCAL_NODE, 0, communicationSystemChid, _NTO_SIDE_CHANNEL, 0);
        if (coid != -1) {
            MsgSend(coid, &msg, sizeof(msg), NULL, NULL);
            ConnectDetach(coid);
        } else {
            std::cerr << "Failed to send data to Communication System." << std::endl;
        }

        // Always reply to unblock the sender
        MsgReply(rcvid, EOK, NULL, 0);
    }

    if (rcvid == -1 && errno != EAGAIN) {
        std::cerr << "MsgReceive failed: " << strerror(errno) << std::endl;
    }
}

void ComputerSystem::checkForViolations() {
    std::lock_guard<std::mutex> guard(stateMutex); // Ensure thread-safe access

    // Iterate through all pairs of aircraft to check for potential violations
    for (const auto& [id1, state1] : aircraftStates) {
        for (const auto& [id2, state2] : aircraftStates) {
            if (id1 == id2) continue; // Skip comparing an aircraft with itself

            bool violationDetectedForPair = false; // Flag to track if a violation is detected for the current pair

            // Check at each second within the next 3 minutes (180 seconds)
            for (int futureTime = 1; futureTime <= 180; ++futureTime) {
                auto futurePos1 = state1.predictPosition(futureTime);
                auto futurePos2 = state2.predictPosition(futureTime);

                int dx = futurePos1.posX - futurePos2.posX;
                int dy = futurePos1.posY - futurePos2.posY;
                int dz = futurePos1.posZ - futurePos2.posZ;

                double distanceXY = std::sqrt(dx * dx + dy * dy); // Calculate XY plane distance
                double distanceZ = std::abs(dz); // Calculate Z axis distance

                // Check if aircraft are within violation distances on both the XY plane and Z axis
                if (distanceZ < 1000 && distanceXY < 3000) { // Check if too close on both Z axis and XY plane
                    // A potential collision has been detected
                    ViolationMsg vmsg;
                    vmsg.messageType = 2; // Identifier for violation messages
                    vmsg.aircraftID1 = id1;
                    vmsg.aircraftID2 = id2;
                    vmsg.posX1 = futurePos1.posX;
                    vmsg.posY1 = futurePos1.posY;
                    vmsg.posZ1 = futurePos1.posZ;
                    vmsg.posX2 = futurePos2.posX;
                    vmsg.posY2 = futurePos2.posY;
                    vmsg.posZ2 = futurePos2.posZ;

                    // Send the violation message to the Operator Console
                    int coid = ConnectAttach(ND_LOCAL_NODE, 0, operatorConsoleChid, _NTO_SIDE_CHANNEL, 0);
                    if (coid != -1) {
                        MsgSend(coid, &vmsg, sizeof(vmsg), NULL, NULL);
                        ConnectDetach(coid);
                        violationDetectedForPair = true; // Mark that a violation has been detected for this pair
                        break; // Stop checking this pair after finding a potential collision
                    } else {
                        std::cerr << "Failed to send violation data to operator console." << std::endl;
                    }
                }
            }

            if (violationDetectedForPair) continue; // If a violation was detected for this pair, move to the next pair
        }
    }
}

void ComputerSystem::logAirspace() {
    if (!active.load()) return;

    struct _msg_info msgInfo;
    int rcvid;
    PositionUpdateMsg msg;

    while ((rcvid = MsgReceive(chid, &msg, sizeof(msg), &msgInfo)) != -1) {
        // Open the file for appending
        int fd = open("/data/home/qnxuser/airspace.log", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            std::cerr << "Failed to open airspace log file: " << strerror(errno) << std::endl;
            return;
        }

        // Construct the airspace information string using data from the PositionUpdateMsg
        std::string airspaceInfo = "Aircraft ID: " + std::to_string(msg.aircraftID) + "\n";
        airspaceInfo += "Position: (" + std::to_string(msg.posX) + ", " + std::to_string(msg.posY) + ", " + std::to_string(msg.posZ) + ")\n";
        airspaceInfo += "Velocity: (" + std::to_string(msg.velX) + ", " + std::to_string(msg.velY) + ", " + std::to_string(msg.velZ) + ")\n\n";

        // Write airspace information to the log file
        ssize_t bytesWritten = write(fd, airspaceInfo.c_str(), airspaceInfo.size());
        if (bytesWritten == -1) {
            std::cerr << "Error writing to airspace log file: " << strerror(errno) << std::endl;
        }

        // Close the file
        close(fd);

        // Always reply to unblock the sender
        MsgReply(rcvid, EOK, NULL, 0);
    }

    if (rcvid == -1 && errno != EAGAIN) {
        std::cerr << "MsgReceive failed: " << strerror(errno) << std::endl;
    }
}


