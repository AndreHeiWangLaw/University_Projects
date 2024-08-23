#include "Timer.h"

Timer::Timer() : timerId(0) {
    memset(&event, 0, sizeof(event));
    memset(&itime, 0, sizeof(itime));
}

Timer::~Timer() {
    stopTimer();
}

void Timer::startTimer(long long nsec, std::function<void()> callback) {
    this->callbackFunction = callback;

    // Setup event for the callback
    SIGEV_THREAD_INIT(&event, &Timer::timerCallback, (void*)this, NULL);

    // Create the timer
    if (timer_create(CLOCK_REALTIME, &event, &timerId) == -1) {
        throw std::runtime_error("Failed to create timer");
    }

    // Specify the timer's time
    itime.it_value.tv_sec = nsec / 1000000000LL;
    itime.it_value.tv_nsec = nsec % 1000000000LL;
    itime.it_interval.tv_sec = itime.it_value.tv_sec;  // For periodic timer
    itime.it_interval.tv_nsec = itime.it_value.tv_nsec;

    // Start the timer
    if (timer_settime(timerId, 0, &itime, NULL) == -1) {
        throw std::runtime_error("Failed to start timer");
    }
}

void Timer::stopTimer() {
    if (timerId) {
        timer_delete(timerId);
        timerId = 0;
    }
}

void Timer::timerCallback(union sigval sv) {
    Timer* timer = (Timer*)sv.sival_ptr;
    if (timer->callbackFunction) {
        timer->callbackFunction();
    }
}
