#include "headers/vecManip.hpp"
#include "headers/classBody.hpp"
#include <fstream>
#include <iostream>

#define G -6.67384e-11
#define PERIOD 5828.8


void updateStates();
std::vector<std::vector<double>> netForce(double prodMass);
void prop(std::vector<double> &pos, std::vector<double> &vel, std::vector<double> force, double dt, int i);


int main(){

	//Body Init &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	//Earth ======================================+
	Body<double> earth("Earth", 5.97e24, {0, 0, 0}, {0, 100000, 0});
	bodies.push_back(earth);
	
	//Uranus ======================================+
	Body<double> uranus("Uranus", 50000, {-7e6, 0, 0}, {0, -754500.687*2, 0});
	bodies.push_back(uranus);
	
	//Satellite ======================================+
	Body<double> sat("Benley", 50000, {7e6, 0, 0}, {0, 754500.687*2, 0});
	bodies.push_back(sat);
	
	//End Body Init &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	
	
	//prodMass init
	double prodMass = 1.0;
	for(size_t i=0; i<bodies.size(); ++i){
		if(bodies[i].getMass()!=0) prodMass *= bodies[i].getMass(); // Producing product of masses
		else{ // If one mass is zero
			std::cerr << "ERROR: Mass of body " << bodies[i].getName() << "[Index: " << i << "] is zero...\nTerminating...\n\n"; 
			exit(1);
		} 
	}
	
	
	//netForce init
	std::vector<std::vector<double>> forces;

	//Time init
	double t = 0.0, dt = 0.002, ElapT=0.0;

	//File output
	std::ofstream fout("SystemStates.txt");

	//tempVals init
	std::vector<double> posTemp = {0, 0, 0};
	std::vector<double> velTemp = {0, 0, 0};

	// Integrating Loop
	for(double i=0; i<100000; ++i){ // Full propagated iteration
		
		//Running only with sat
		forces = netForce(prodMass);
		
		for(size_t j=0;j<bodies.size();++j){ // Per-body propagation
			// Reassign Temps
			posTemp = bodies[j].getPos();
			velTemp = bodies[j].getVel();
			// Integrate
			prop(posTemp, velTemp, forces[j], dt, j);
			// setPos and setVel Update
			bodies[j].setPosNew(posTemp);
			bodies[j].setVelNew(velTemp);
		}
		
		// Update time
		ElapT += dt;
		t += dt;

		// Update positions
		updateStates();

		// File output
		for(size_t j=0;j<bodies.size();++j){
			//Pos
			for(unsigned short k = 0; k < 3; ++k){
				fout << bodies[j].getPos()[k] << '\t';
			}
			//Vel
			for(unsigned short k = 0; k < 3; ++k){
				fout << bodies[j].getVel()[k] << '\t';
			}
		}
		fout << ElapT << '\n';
		
	}

        return 0;

}

// Updates old positions to new positions
void updateStates(){
	for(size_t i=0;i<bodies.size();++i){
		bodies[i].setPos(bodies[i].getPosNew());
		bodies[i].setVel(bodies[i].getVelNew());
	}
}

// Calculates netForce vectors for all bodies in system
std::vector<std::vector<double>> netForce(double prodMass){
	
	size_t n = bodies.size();
	std::vector<double> dist = {0, 0, 0};
	std::vector<std::vector<double>> forces;
	
	for(unsigned int i = 0; i < n; ++i){
		
		for(unsigned int j = 0; j < n; ++j){
			if(i != j){
				double magnitude = mag(VecSub(bodies[i].getPos(), bodies[j].getPos()));
				dist = VecAdd(dist, VecScaleMulti((VecSub(bodies[i].getPos(), bodies[j].getPos())), (1.0/(magnitude * magnitude * magnitude))));
			}
		}
		forces.push_back(VecScaleMulti(dist, prodMass*G));
		zeroVec(dist);
	}
	
	return forces;

}

// Propagates bodies (1st order)
void prop(std::vector<double> &pos, std::vector<double> &vel, std::vector<double> force, double dt, int i){
	// Updating Pnuementum
	vel = VecAdd(vel, VecScaleMulti(force, dt/bodies[i].getMass()));
	
	// Updating Pos
	pos=VecAdd(pos, VecScaleMulti(vel, dt));
}