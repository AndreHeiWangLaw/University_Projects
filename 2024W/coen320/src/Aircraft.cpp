#include "Aircraft.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/netmgr.h>

#include "Radar.h"

Aircraft::Aircraft(int id, double posX, double posY, double posZ, double velX, double velY, double velZ, int radarChid)
: id(id), posX(posX), posY(posY), posZ(posZ), velX(velX), velY(velY), velZ(velZ), active(false), radarChid(radarChid) {
}

Aircraft::~Aircraft() {
    stop();
}

void Aircraft::start() {
    active = true;
    // Start the timer with a callback to update and send position
    positionUpdateTimer.startTimer(1000000000LL, [this]() {
        this->updatePosition();
        this->sendPositionUpdate();
    });
}

void Aircraft::stop() {
    active = false;
    positionUpdateTimer.stopTimer();
}

void Aircraft::updatePosition() {
	posX += velX;
	posY += velY;
	posZ += velZ;
}

void Aircraft::sendPositionUpdate() {
	PositionUpdateMsg msg;
    msg.messageType = 1; // Identifying as a position update message
    msg.aircraftID = id;
    msg.posX = static_cast<int>(posX); // Send position information 
    msg.posY = static_cast<int>(posY);
    msg.posZ = static_cast<int>(posZ);
    msg.velX = static_cast<int>(velX); // Send velocity information 
    msg.velY = static_cast<int>(velY);
    msg.velZ = static_cast<int>(velZ);

    int coid = ConnectAttach(ND_LOCAL_NODE, 0, radarChid, _NTO_SIDE_CHANNEL, 0);
    if (coid != -1) {
        MsgSend(coid, &msg, sizeof(msg), NULL, NULL);
        ConnectDetach(coid);
    } else {
        std::cerr << "Failed to connect to Radar's channel" << std::endl;
    }
}
