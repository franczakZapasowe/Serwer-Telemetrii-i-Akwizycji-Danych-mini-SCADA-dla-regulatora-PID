#include "ThreadPool.h"
#include "PIDTelemetryPayload.h"
#include <iostream>

void ThreadPool::workerLoop() {
    PIDTelemetryPayload payload;
    while (running.load()) {
        m_safeQ->pop(payload);
        std::cout<<"Packet_id: "<<payload.packet_id<<" pv: "<<payload.process_variable<<" error: "<<payload.error<<" time: "<<payload.timestamp_ms<<" ms\n";
    }
}
