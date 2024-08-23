#ifndef OPERATORCONSOLE_H_
#define OPERATORCONSOLE_H_

#include <thread>
#include <atomic>
#include <mutex>
#include <sys/neutrino.h>
#include "ComputerSystem.h" // For ViolationMsg structure

class OperatorConsole {
private:
    std::atomic<bool> active; // Operator Console's operational state
    std::thread worker; // Thread handling the message receiving and printing
    int chid; // Channel ID for receiving data from the Computer System
    std::mutex printMutex;

    void messageReceiver(); // Function to receive and process messages

public:
    OperatorConsole();
    ~OperatorConsole();
    void start();
    void stop();
    int getChid() const { return chid; }
    static std::mutex& getPrintMutex();
};

#endif // OPERATORCONSOLE_H_
