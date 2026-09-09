#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include "PIDTelemetryPayload.h"

class ThradeSafeQ {
    std::mutex mutex;
    std::queue<PIDTelemetryPayload>kolejka;
    std::condition_variable cv;
    bool flag{false};
public:
    void push(PIDTelemetryPayload p);
    void pop(PIDTelemetryPayload& p);
};

