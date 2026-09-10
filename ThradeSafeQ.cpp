#include "ThradeSafeQ.h"
#include <iostream>

#include "ThreadPool.h"

void ThradeSafeQ::push(PIDTelemetryPayload p) {
    std::unique_lock<std::mutex> lock(mutex);
    kolejka.push(p);
    cv.notify_one();
}

bool ThradeSafeQ::pop(PIDTelemetryPayload &p) {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock,[this]{return !kolejka.empty() || closeStatus; });
        if (closeStatus && kolejka.empty()) {
            return false;
        }
        p = kolejka.front();
        kolejka.pop();
        return true;
}

void ThradeSafeQ::abort() {
    std::unique_lock<std::mutex> lock(mutex);
    closeStatus = true;
    cv.notify_all();
}


