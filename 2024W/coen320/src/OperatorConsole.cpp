#include "OperatorConsole.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/netmgr.h>

OperatorConsole::OperatorConsole() : active(false), chid(-1) {
    chid = ChannelCreate(0);
    if (chid == -1) {
        std::cerr << "ChannelCreate failed: " << strerror(errno) << std::endl;
    }
}

OperatorConsole::~OperatorConsole() {
    stop();
    if (chid != -1) {
        ChannelDestroy(chid);
    }
}

void OperatorConsole::start() {
    if (!active) {
        active = true;
        worker = std::thread(&OperatorConsole::messageReceiver, this);
    }
}

void OperatorConsole::stop() {
    if (active) {
        active = false;
        worker.join();
    }
}

void OperatorConsole::messageReceiver() {
    while (active) {
        struct _msg_info msgInfo;
        ViolationMsg vmsg;
        int rcvid = MsgReceive(chid, &vmsg, sizeof(vmsg), &msgInfo);
        if (rcvid == -1) {
            std::cerr << "MsgReceive failed: " << strerror(errno) << std::endl;
            continue;
        }

        // Check if the messageType is for a violation and all coordinates are non-negative
        if (vmsg.messageType == 2 && vmsg.posX1 >= 0 && vmsg.posY1 >= 0 && vmsg.posX2 >= 0 && vmsg.posY2 >= 0 && vmsg.posX1 <= 100000 && vmsg.posY1 <= 100000 && vmsg.posX2 <= 100000 && vmsg.posY2 <= 100000 ) {

            std::lock_guard<std::mutex> guard(getPrintMutex());
            std::cout << "\n*** Violation Alert ***\n";
            std::cout << "Aircraft " << vmsg.aircraftID1 << " and Aircraft " << vmsg.aircraftID2
                      << " are predicted to violate separation standards within the next 3 minutes.\n";
            std::cout << "Predicted Collision Point: \n";
            std::cout << " - Location of Aircraft " << vmsg.aircraftID1 << " (X: " << vmsg.posX1 << ", Y: " << vmsg.posY1 << ", Z: " << vmsg.posZ1 << ")\n";
            std::cout << " - Location of Aircraft " << vmsg.aircraftID2 << " (X: " << vmsg.posX2 << ", Y: " << vmsg.posY2 << ", Z: " << vmsg.posZ2 << ")\n\n";
        }

        MsgReply(rcvid, EOK, NULL, 0);
    }
}



std::mutex& OperatorConsole::getPrintMutex() {
    static std::mutex mtx;
    return mtx;
}
