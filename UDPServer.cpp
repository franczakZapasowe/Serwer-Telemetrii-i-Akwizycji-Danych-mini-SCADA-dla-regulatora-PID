#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "UDPServer.h"
#include "PIDTelemetryPayload.h"
#include "ThradeSafeQ.h"
#include "ThreadPool.h"
void UDPServer::startListening() {

    int serwerSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serwerSocket == -1) {
        std::cerr<<"Fail to create serwer socket\n";
        exit(1);
    }
    std::cerr<<"[INFO] SOCKET CREATED SUCCESFULLY\n";
    sockaddr_in serwerSockAddr{};
    serwerSockAddr.sin_family = AF_INET;
    serwerSockAddr.sin_port = htons(m_port);
    serwerSockAddr.sin_addr.s_addr = INADDR_ANY;

    int ifBindd{};
    if ( (ifBindd = bind(serwerSocket,reinterpret_cast<sockaddr*>(&serwerSockAddr),sizeof(serwerSockAddr))) == -1) {
        std::cerr<<"Fail to bind\n";
        exit(1);
    }
    std::cerr<<"[INFO] SOCKET BINDED SUCCESFULLY\n";

    sockaddr_in clientAddr;

    char buffor[1024]{};
    PIDTelemetryPayload pidPayload{};
    int recvBytes{};
    ThradeSafeQ kolejka{};
    // ThreadPool threadPool(&kolejka);
    // std::thread t1 (threadPool.workerLoop);


    while (true) {
        // wywoluje przed kazdym nadpisaniem, zeby nie bylo bled gdy np przyjdzie za maly packet
        socklen_t clientAddrLen = sizeof(clientAddr);
        recvBytes = recvfrom(serwerSocket,buffor,sizeof(PIDTelemetryPayload),0,reinterpret_cast<sockaddr*>(&clientAddr),&clientAddrLen);
        if (recvBytes < 0) {
            std::cerr<<"[WARNING] EMPTY PACKED\n";
            continue;
        }
        if (recvBytes < sizeof(PIDTelemetryPayload)) {
            std::cerr<<"[WARNING] INCOMPLETE PACKED: "<<recvBytes<<" /" <<sizeof(PIDTelemetryPayload)<<"\n";
            continue;
        }
        std::memcpy(&pidPayload,buffor,sizeof(PIDTelemetryPayload)); //kopia binarna
        if (pidPayload.status_flags==0) break; // alarm
        kolejka.push(pidPayload);
    }
    close(serwerSocket);
    //t1.join();
}
