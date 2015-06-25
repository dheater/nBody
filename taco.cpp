#include "headers/vecManip.hpp"
#include "headers/RK4.hpp"
#include "headers/classBody.hpp"
#include <fstream>
#include <iostream>

#define G -6.67384e-11
#define PERIOD 5828.8

void updateStates();
std::vector<std::vector<double>> netForce();

void prop(std::vector<double> &pos, std::vector<double> &vel, double dt, int i){
	// Updating Distance Between Bodies
	double r = mag(VecSub(pos, bodies[i].getPos()));
	
	// Updating Net Force
	std::vector<double> fNet = VecScaleMulti((VecSub(pos, bodies[i].getPos())), (G*bodies[0].getMass()*bodies[1].getMass())/(r*r*r)); // Update Netforce
			
	// Updating Pmomentum
	vel = VecAdd(vel, VecScaleMulti(fNet, dt/bodies[(i+1)%2].getMass()));
	
	// Updating Pos
	pos=VecAdd(pos, VecScaleMulti(vel, dt));
}



int main(){

	//Body Init=================+

	//Earth =========NAME==MASS===POS====VEL======+
	Body<double> earth("Earth", 5.97e24, {10, 10, 0}, {0, 0, 0});
	bodies.push_back(earth);

	//Satellite =======NAME=MASS===POS======VEL=======+ 7545.687
	Body<double> sat("Benley", 5000, {7e6, 0, 0}, {0, 7545.687, 0});
	bodies.push_back(sat);

	//End Body Init=============+
	
	
	
	std::vector<std::vector<double>> forcesBro = netForce();
	
	for(int i = 0; i < forcesBro.size(); ++i){
		for(int j = 0; j < 3; ++j){
			std::cout << forcesBro[i][j] << " ";
		}
		std::cout << '\n';
	}
	
	

	//Time init
	double t = 0.0, dt = 0.2, ElapT=0.0;

	std::cout << "bodies.size(): " << bodies.size() << std::endl;

	//File output
	std::ofstream fout("SystemStates.txt");

	//Integration Loop
	std::vector<double> posNewTemp = {0, 0, 0};
	std::vector<double> velNewTemp = {0, 0, 0};
	std::vector<double> posTemp = {0, 0, 0};
	std::vector<double> velTemp = {0, 0, 0};
	//double massTemp = 0;

	for(double i=0; i<100000; ++i){ // Full propagated iteration
		//Running only with sat
		for(size_t j=0;j<bodies.size();++j){ // Per-body propagation
			// Reassign Temps
			posTemp = bodies[j].getPos();
			velTemp = bodies[j].getVel();
			//massTemp = bodies[j].getMass();
			// Integrate
			prop(posTemp, velTemp, dt, (j-1)%2);
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
		fout << bodies[1].getPos()[0] << '\t' << bodies[1].getPos()[1] << '\t' << bodies[1].getPos()[2] << '\t'
			<< bodies[0].getPos()[0] << '\t' << bodies[0].getPos()[1] << '\t' << bodies[0].getPos()[2] << '\t'
			<< bodies[1].getVel()[0] << '\t' << bodies[1].getVel()[1] << '\t' << bodies[1].getVel()[2] << '\t'
			<< ElapT << '\n';
	}

        return 0;

}

void updateStates(){
	for(size_t i=0;i<bodies.size();++i){
		bodies[i].setPos(bodies[i].getPosNew());
		bodies[i].setVel(bodies[i].getVelNew());
	}
}

std::vector<std::vector<double>> netForce(){
	
	size_t n = bodies.size();
	double totMass = 1.0;
	std::vector<double> pos = {0, 0, 0};
	std::vector<std::vector<double>> output;
	
	for(unsigned int i = 0; i < n; ++i){
		totMass *= bodies[i].getMass();
		for(unsigned int j = 0; j < n && i!=j; ++j){
			double magnitude = mag(VecSub(bodies[i].getPos(), bodies[j].getPos()));
			pos = VecAdd(pos, VecScaleMulti((VecSub(bodies[i].getPos(), bodies[j].getPos())), (1.0/(magnitude * magnitude * magnitude))));
		}
		output.push_back(VecScaleMulti(pos, totMass*G));
	}
	return output;
}