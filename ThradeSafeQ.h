#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include "PIDTelemetryPayload.h"

class ThradeSafeQ {
    std::mutex mutex;
    std::queue<PIDTelemetryPayload>kolejka;
    std::condition_variable cv;
    bool flag{false};
    bool closeStatus{false};
public:
    ThradeSafeQ() = default;
    void push(PIDTelemetryPayload p);
    bool pop(PIDTelemetryPayload& p);
    void abort();
};

