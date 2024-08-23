#include "Radar.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/netmgr.h>

Radar::Radar() : active(false), chid(-1), computerSystemChid(-1) {
    // Create a channel for aircraft to send updates
    chid = ChannelCreate(0);
    if (chid == -1) {
        std::cerr << "ChannelCreate failed: " << strerror(errno) << std::endl;
    }
}

Radar::~Radar() {
    stop();
    if (chid != -1) {
        ChannelDestroy(chid);
    }
}

void Radar::start() {
    if (!active.load()) {
        active = true;
        // Setup timer to periodically invoke handleIncomingMessages
        radarTimer.startTimer(1000000000LL, [this]() {
            this->handleIncomingMessages();
        });
    }
}

void Radar::stop() {
    if (active.load()) {
        active = false;
        radarTimer.stopTimer();
    }
}

void Radar::setComputerSystemChid(int chid) {
    this->computerSystemChid = chid;
}

void Radar::handleIncomingMessages() {
    if (!active.load()) return;

    struct _msg_info msgInfo;
    int rcvid;
    PositionUpdateMsg msg;

    while ((rcvid = MsgReceive(chid, &msg, sizeof(msg), &msgInfo)) != -1) {

        // Check if the aircraft is within the boundaries
        if (msg.posX >= 0 && msg.posX <= 100000 && msg.posY >= 0 && msg.posY <= 100000 && msg.posZ >= 15000 && msg.posZ <= 25000) {
            // Forward the message to the Computer System
            int coid = ConnectAttach(ND_LOCAL_NODE, 0, computerSystemChid, _NTO_SIDE_CHANNEL, 0);
            if (coid != -1) {
                MsgSend(coid, &msg, sizeof(msg), NULL, NULL);
                ConnectDetach(coid);
            } else {
                std::cerr << "Failed to send data to Computer System." << std::endl;
            }
        }
        // Always reply to unblock the sender
        MsgReply(rcvid, EOK, NULL, 0);
    }

    if (rcvid == -1 && errno != EAGAIN) {
        std::cerr << "MsgReceive failed: " << strerror(errno) << std::endl;
    }
}

