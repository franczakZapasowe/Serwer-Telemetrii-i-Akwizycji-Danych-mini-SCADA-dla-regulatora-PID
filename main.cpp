#include <iostream>
#include "PID.h"
#include "ProcessSimulator.h"

int main(){

	PID pid(0.1f,0.0f,0.0f,0.5f, -1.0f, 1.0f, -50.0f, 50.0f);
	ProcessSimulator pv(0.1);

	float sp = 2.0f;
	for (int i =0; i<10000;i++) {
		pv.updatePv(pid.getOutput()); // tu potrzbuje wyniku z poprzedniego pid update
		pid.update(sp, pv.getPV()); // a tu potrzebuje
		std::cout<<"Iteracja "<<i<<" SP: "<<sp<<" PV: "<<pv.getPV()<<" ControlOutput"<<pid.getOutput()<<" ERROR "<< pid.getError()<<"\n";	}
	return 0;
}
