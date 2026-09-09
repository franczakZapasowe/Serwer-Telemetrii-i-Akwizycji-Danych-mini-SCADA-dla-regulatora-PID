#include <iostream>
#include "PID.h"
#include "ProcessSimulator.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <cstring>

#include "PIDTelemetryPayload.h"
#include <thread>

PIDTelemetryPayload spakuj(uint32_t id,std::chrono::milliseconds duration, float sp, float pv, float output,float error,uint16_t status_flags,uint16_t checksum  ) {
	PIDTelemetryPayload pidPayload{};

	pidPayload.packet_id = id;
	pidPayload.timestamp_ms = duration.count();
	pidPayload.setpoint = sp;
	pidPayload.process_variable = pv;
	pidPayload.control_output = output;
	pidPayload.error = error;
	pidPayload.status_flags = status_flags;
	pidPayload.checksum = checksum;

	return pidPayload;
}

int main(){

	int clientSocket = socket(AF_INET,SOCK_DGRAM,0);
	if (clientSocket == -1) {
		std::cerr<<"FAIL TO CREATE SOCKET\n";
		exit(1);
	}
    std::cerr<<"[INFO] SOCKET CRATED SUCCESFULLY\n";

	sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(4000);
	inet_pton(AF_INET,"127.0.0.1",&serverAddr.sin_addr);


	PID pid(0.1f,0.0f,0.0f,-1.0f, 1.0f, -50.0f, 50.0f);
	ProcessSimulator pv(0.1);
	float sp = 2.0f;

	auto start_of_simulation = std::chrono::steady_clock::now(); // dla obliczania czasu calej symualacji
	auto last_time = std::chrono::steady_clock::now();  // czas przed symulacja dla obliczania pojedynczej iteracji
	float dt{0.3f}; // dt
	for (int i = 0; i<100;i++) {
		auto iteration_time = std::chrono::steady_clock::now(); // koniec dla jednej iteracji
		dt = std::chrono::duration<float>(iteration_time - last_time).count();
		last_time = iteration_time;
		pv.updatePv(pid.getOutput());
		pid.update(sp, pv.getPV(),dt);

		const auto current_time = std::chrono::steady_clock::now(); // czas potrzebny dla obliczenia jednego wyniku
		auto timestamp_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_of_simulation); // czas od poczatku symulacji do teraz

		PIDTelemetryPayload pidPayload = spakuj(i,timestamp_ms_duration,sp,pv.getPV(),pid.getOutput(),pid.getError(),1,0); // 2 ostatnie elemnty dalem na 0 bo nie wiem jhak maja dzialac jak dasz mi wymogi do nich to zmeinie to
		int sendStatus = sendto(clientSocket, reinterpret_cast<const char *>(&pidPayload), sizeof(PIDTelemetryPayload),
		                        0, reinterpret_cast<sockaddr *>(&serverAddr), serverAddrLen);
		if (sendStatus == -1) {
			std::cerr<<"FAIL TO SEND DATA\n";
			exit(1);
		}
		std::cout<<"Iteracja "<<i<<" SP: "<<sp<<" PV: "<<pv.getPV()<<" ControlOutput"<<pid.getOutput()<<" ERROR "<< pid.getError()<<"\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
		PIDTelemetryPayload pidPayload = spakuj(0,std::chrono::milliseconds(0),0,0,0,0,0,0); // 2 ostatnie elemnty dalem na 0 bo nie wiem jhak maja dzialac jak dasz mi wymogi do nich to zmeinie to
		sendto(clientSocket, reinterpret_cast<const char *>(&pidPayload), sizeof(PIDTelemetryPayload),
							0, reinterpret_cast<sockaddr *>(&serverAddr), serverAddrLen);

	return 0;
}
