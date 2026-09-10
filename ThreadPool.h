#pragma once
#include <mutex>
#include "ThradeSafeQ.h"
class ThreadPool {
    std::mutex m;
    ThradeSafeQ * m_safeQ;
public:
    ThreadPool(ThradeSafeQ * safeQ):m_safeQ(safeQ) {};
    void workerLoop();
};


