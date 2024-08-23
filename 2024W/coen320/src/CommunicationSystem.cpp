#include "CommunicationSystem.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>

CommunicationSystem::CommunicationSystem() : active(false), chid(-1) {
    chid = ChannelCreate(0);
    if (chid == -1) {
        std::cerr << "ChannelCreate failed: " << strerror(errno) << std::endl;
    }
}

CommunicationSystem::~CommunicationSystem() {
    stop();
    if (chid != -1) {
        ChannelDestroy(chid);
    }
}

void CommunicationSystem::start() {
    if (!active.load()) {
        active = true;
        try {
            communicationTimer.startTimer(2000000000LL, [this]() {
                this->handleIncomingMessages();
            });
        } catch (const std::runtime_error& e) {
            std::cerr << "Timer error: " << e.what() << std::endl;
        }
    }
}

void CommunicationSystem::stop() {
    if (active.load()) {
        active = false;
        communicationTimer.stopTimer();
    }
}

void CommunicationSystem::handleIncomingMessages() {
    if (!active) return;
    struct _msg_info msgInfo;
    int rcvid;

    PositionUpdateMsg msg;

    while ((rcvid = MsgReceive(chid, &msg, sizeof(msg), &msgInfo)) != -1) {

        MsgReply(rcvid, EOK, NULL, 0); 
    }

    if (rcvid == -1 && errno != EAGAIN) {
        std::cerr << "MsgReceive failed: " << strerror(errno) << '\n';
    }
}

int CommunicationSystem::getChid() const {
    return chid;
}

