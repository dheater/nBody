#include "headers/vecManip.hpp"
#include "headers/RK4.hpp"
#include "headers/classBody.hpp"
#include <fstream>
#include <iostream>

#define G -6.67384e-11
#define PERIOD 5828.8

std::vector<double> accelFunc(std::vector<double>, std::vector<double>, double);

void updateStates();

int main(){
	
	//Body Init=================+
	
	//Earth =========NAME==MASS===POS====VEL======+
	Body<double> earth("Earth", 5.97e24, {0, 0, 0}, {0, 0, 0});
	bodies.push_back(earth);
	
	//Satellite =======NAME=MASS===POS======VEL=======+ 7545.687
	Body<double> sat("Benley", 5000, {7e6, 0, 0}, {0, 7545.687, 0});
	bodies.push_back(sat);
	
	//End Body Init=============+
	
	//Time init
	double t = 0.0, dt = 0.2, ElapT=0.0;

	std::cout << "bodies.size(): " << bodies.size() << std::endl;
	
	//File output 
	std::ofstream fout("SystemStates.txt");
	
	//Integration Loop
	std::vector<double> posNewTemp = {0,0,0};
	std::vector<double> velNewTemp = {0,0,0};
	std::vector<double> posTemp = {0, 0, 0};
	std::vector<double> velTemp = {0, 0, 0};
	
	for(double i=0; i<100000; ++i){ // Full propagated iteration
		
		//Running only with sat
		for(size_t i=0;i<bodies.size();++i){ // Per-body propagation
			// Zero / Reassign Temps
			zeroVec(posNewTemp);
			zeroVec(velNewTemp);
			posTemp = bodies[i].getPos();
			velTemp = bodies[i].getVel();
			// Integrate
			evaluate(accelFunc, posTemp, velTemp, t, dt, posNewTemp, velNewTemp); 
			// setPos and setVel Update
			bodies[i].setPosNew(posNewTemp);
			bodies[i].setVelNew(velNewTemp);
		}
		// Update time
		ElapT += dt;
		
		// Update positions
		updateStates();
		
		// File output
		fout << bodies[1].getPos()[0] << '\t' << bodies[1].getPos()[1] << '\t' << bodies[1].getPos()[2] << '\t'
			<< bodies[0].getPos()[0] << '\t' << bodies[0].getPos()[1] << '\t' << bodies[0].getPos()[2] << '\t'
			<< bodies[1].getVel()[0] << '\t' << bodies[1].getVel()[1] << '\t' << bodies[1].getVel()[2] << '\t'
			<< ElapT << '\n';
		
		//bodies[1].setPos(bodies[1].getPosNew());
		//bodies[1].setVel(bodies[1].getVelNew());
	}

        return 0;
	
}

void updateStates(){
	for(size_t i=0;i<bodies.size();++i){
		bodies[i].setPos(bodies[i].getPosNew());
		bodies[i].setVel(bodies[i].getVelNew());
	}
}

std::vector<double> accelFunc(std::vector<double> pos, std::vector<double> vel, double t){
	
	std::vector<double> accel = {0, 0, 0};
	
	double r;
	
	for(size_t i=0; i<bodies.size();++i){
		r = mag(VecSub(pos, bodies[i].getPos()));
		if(r==0){ // If comparing body is same as current body
			continue;
		}else{ // If comparing to foreign body
			//accel+=bodies[i].getMass()/(r*r*r);
			accel= VecAdd(accel, VecScaleMulti(VecSub(pos, bodies[i].getPos()), (G*bodies[i].getMass()/(r*r*r))  ) );
		} 
	}
	
	return accel;
}