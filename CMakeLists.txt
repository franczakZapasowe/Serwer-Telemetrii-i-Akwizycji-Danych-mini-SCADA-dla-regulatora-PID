cmake_minimum_required(VERSION 3.10)

project(PID)

set(CMAKE_CXX_STANDARD 23)

add_executable(pid main.cpp
        PIDTelemetryPayload.h
        PID.cpp
        PID.h
        ProcessSimulator.h
        UDPServer.cpp
        UDPServer.h
        Serwer.cpp
        ThradeSafeQ.cpp
        ThradeSafeQ.h
        ThreadPool.cpp
        ThreadPool.h
)

