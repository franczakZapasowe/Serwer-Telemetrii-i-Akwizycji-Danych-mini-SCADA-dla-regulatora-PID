//
// Created by mf on 9/8/26.
//
#include "UDPServer.h"

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "PIDTelemetryPayload.h"
void UDPServer::startListening() {

    int serwerSocker = socket(AF_INET, SOCK_DGRAM, 0);
    if (serwerSocker == -1) {
        std::cerr<<"Fail to create serwer socket\n";
        exit(1);
    }
    std::cerr<<"[INFO] SOCKET CREATED SUCCESFULLY\n";
    sockaddr_in serwerSockAddr;
    socklen_t serwerSockAddrLen = sizeof(serwerSockAddr);
    serwerSockAddr.sin_family = AF_INET;
    serwerSockAddr.sin_port = htons(m_port);
    serwerSockAddr.sin_addr.s_addr = INADDR_ANY;

    int ifBindd{};
    if ( (ifBindd = bind(serwerSocker,reinterpret_cast<sockaddr*>(&serwerSockAddr),sizeof(serwerSockAddr))) == -1) {
        std::cerr<<"Fail to bind\n";
        exit(1);
    }
    std::cerr<<"[INFO] SOCKET BINDED SUCCESFULLY\n";

    sockaddr_in clientAddr;

    char buffor[1024]{};
    PIDTelemetryPayload pidPayload{};
    int recvBytes{};
    while (true) {
        // wywoluje przed kazdym nadpisaniem, zeby nie bylo bled gdy np przyjdzie za maly packet
        socklen_t clientAddrLen = sizeof(clientAddr);
        recvBytes = recvfrom(serwerSocker,buffor,sizeof(PIDTelemetryPayload),0,reinterpret_cast<sockaddr*>(&clientAddr),&clientAddrLen);
        if (recvBytes < 0) {
            std::cerr<<"[WARNING] EMPTY PACKED\n";
            continue;
        }
        if (recvBytes < sizeof(PIDTelemetryPayload)) {
            std::cerr<<"[WARNING] INCOMPLETE PACKED: "<<recvBytes<<" /" <<sizeof(PIDTelemetryPayload)<<"\n";
            continue;
        }
        std::memcpy(&pidPayload,buffor,sizeof(PIDTelemetryPayload)); //kopia binarna
        std::cout<<"Packet_id: "<<pidPayload.packet_id<<" pv: "<<pidPayload.process_variable<<" error: "<<pidPayload.error<<" time: "<<pidPayload.timestamp_ms<<" ms\n";
    }
    close(serwerSocker);
}
