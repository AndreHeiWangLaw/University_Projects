#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <sys/siginfo.h>
#include <sys/neutrino.h>

class Timer {
public:
    Timer();
    ~Timer();

    void startTimer(long long nsec, std::function<void()> callback);
    void stopTimer();

private:
    timer_t timerId;
    struct sigevent event;
    struct itimerspec itime;
    std::function<void()> callbackFunction;

    static void timerCallback(union sigval sv);
};

#endif // TIMER_H_
