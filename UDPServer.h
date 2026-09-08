#pragma once

class UDPServer {
    int m_port{};

    public:
    UDPServer(int port):m_port{port} {};
    void startListening();
};


