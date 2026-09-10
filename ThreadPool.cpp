#include "ThreadPool.h"
#include "PIDTelemetryPayload.h"
#include <iostream>
#include  <vector>
void ThreadPool::workerLoop() {
    PIDTelemetryPayload payload;
    std::vector<PIDTelemetryPayload>wektor{};
    while (m_safeQ->pop(payload)) {
        if (wektor.size() == 100) {
            dodajDoBazy(wektor);
        }
        wektor.push_back(payload);
    }
    if (!wektor.empty()) {
        dodajDoBazy(wektor);
    }
}

void ThreadPool::dodajDoBazy(std::vector<PIDTelemetryPayload>& wektor) {
    sqlite3_exec(db, "BEGIN TRANSACTION;",nullptr,nullptr,nullptr);
    for (auto &v : wektor) {
        sqlite3_bind_int(stmt,1,v.packet_id);
        sqlite3_bind_int64(stmt,2,v.timestamp_ms);
        sqlite3_bind_double(stmt,3,v.setpoint);
        sqlite3_bind_double(stmt,4,v.process_variable);
        sqlite3_bind_double(stmt,5,v.control_output);
        sqlite3_bind_double(stmt,6,v.error);
        sqlite3_bind_int(stmt,7,v.status_flags);

        int result = sqlite3_step(stmt);
        if (result != SQLITE_DONE && result != SQLITE_ROW) {
            std::cerr<<"BLAD ZAPISU DO BAZY\n";
        }

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
    sqlite3_exec(db, "COMMIT;",nullptr,nullptr,nullptr);
    wektor.clear();
}
