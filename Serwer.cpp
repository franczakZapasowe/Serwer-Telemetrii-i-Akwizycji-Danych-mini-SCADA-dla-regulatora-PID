#include "UDPServer.h"

int main() {
    UDPServer udpServer (4000);
    udpServer.startListening();

    return 0;
}