#pragma once
#include <iostream>
#include <mutex>
#include <ostream>

#include "ThradeSafeQ.h"
#include <sqlite3.h>
class ThreadPool {
    std::mutex m;
    ThradeSafeQ * m_safeQ;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    void dodajDoBazy(std::vector<PIDTelemetryPayload>&wektor);
public:
    ThreadPool(ThradeSafeQ * safeQ):m_safeQ(safeQ) {
        int status = sqlite3_open("telemtry.db",&db);
        if(status!=SQLITE_OK) {
            std::cerr<<"[DATA BASE ERROR] CAN NOT OPEN DATABASE: "<<sqlite3_errmsg(db)<<"\n";
            exit(1);
        }

        const char * create_table_sql =
            "CREATE TABLE IF NOT EXISTS PidLogs("
            "ID INTEGER, "
            "TimeStamp INTEGER, "
            "SetPoint REAL, "
            "ProcessVariable REAL, "
            "ControlOutput REAL, "
            "Error REAL, "
            "StatusFlag INTEGER);";

        char* error_message = nullptr;
        status = sqlite3_exec(db,create_table_sql,nullptr,nullptr,&error_message);
        if (status!=SQLITE_OK) {
            std::cerr<<"[DATA BASE ERROR] CAN NOT CREATE TABLE: "<<error_message<<"\n";
            sqlite3_free(error_message);
            exit(1);
        }

        const char* sql = "INSERT INTO PidLogs VALUES (?, ?, ?, ?, ?, ?, ?);";
        int rc = sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
        if (rc!=SQLITE_OK) {
            std::cerr<<"ERROR: "<<sqlite3_errmsg(db)<<"\n";
            exit(1);
        }
    }
    ~ThreadPool() {
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt=nullptr;
        }
        sqlite3_close(db);
    }
    void workerLoop();
};


