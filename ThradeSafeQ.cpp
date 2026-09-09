#include "ThradeSafeQ.h"
#include <iostream>

void ThradeSafeQ::push(PIDTelemetryPayload p) {
    std::unique_lock<std::mutex> lock(mutex);
    kolejka.push(p);
    std::cerr<<"[INFO] ADD ITEM TO QUEUE: \n";
    flag = true;
    cv.notify_one();
}

void ThradeSafeQ::pop(PIDTelemetryPayload &p) {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock,[this]{return flag || !kolejka.empty(); });
    p = kolejka.front();
    kolejka.pop();
}


