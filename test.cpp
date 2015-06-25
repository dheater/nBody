#include "headers/vecManip.hpp"
#include "headers/RK4.hpp"
#include <fstream>
#include <iostream>

#define G -6.67384e-11
#define PERIOD 5828.8


//Acceleration Function
std::vector<double> accelFunc(std::vector<double>, std::vector<double>, double);

//State Update Function
void updateStates();




//Body struct
struct bod{
	double mass;
	std::vector<double> pos;
	std::vector<double> vel;
	std::vector<double> posNew;
	std::vector<double> velNew;
};

std::vector<bod> bods;






int main(){
	//Body Init=================+

	//Earth =========NAME==MASS===POS====VEL======+
	bod earth;
	earth.mass = 5.97e24;
	earth.pos = {0, 0, 0};
	earth.vel = {0, 0, 0};
	earth.posNew = {0, 0, 0};
	earth.velNew = {0, 0, 0};
	bods.push_back(earth);

	//Satellite =======NAME=MASS===POS======VEL=======+ 7545.687
	bod sat;
	sat.mass = 5000;
	sat.pos ={7e6, 0, 0};
	sat.vel ={0, 7545.687, 0};
	sat.posNew = {0, 0, 0};
	sat.velNew = {0, 0, 0};
	bods.push_back(sat);

	//End Body Init=============+







	//Time init
	double t = 0.0, dt = 50.2, ElapT=0.0;

	std::cout << "bods.size(): " << bods.size() << std::endl;

	//File output
	std::ofstream fout("SystemStates.txt");






	for(double i=0; i<100000; ++i){ // Full propagated iteration

		//Running only with sat
		for(size_t j=0;j<bods.size();++j){ // Per-body propagation
			evaluate(accelFunc, bods[j].pos, bods[j].vel, t, dt, bods[j].posNew, bods[j].velNew);
		}

		// Update time
		ElapT += dt;

		// Update positions
		updateStates();

		// File output
		fout << bods[1].pos[0] << '\t' << bods[1].pos[1] << '\t' << bods[1].pos[2] << '\t'
			<< bods[0].pos[0] << '\t' << bods[0].pos[1] << '\t' << bods[0].pos[2] << '\t'
			<< bods[1].vel[0] << '\t' << bods[1].vel[1] << '\t' << bods[1].vel[2] << '\t'
			<< ElapT << '\n';
	}

        return 0;

}

void updateStates(){
	for(size_t i=0;i<bods.size();++i){
		bods[i].pos = bods[i].posNew;
		bods[i].vel = bods[i].velNew;
	}
}

std::vector<double> accelFunc(std::vector<double> pos, std::vector<double> vel, double t){

	std::vector<double> accel = {0, 0, 0};

	double r;

	for(size_t i=0; i<bods.size();++i){
		r = mag(VecSub(pos, bods[i].pos));
		if(r==0){ // If comparing body is same as current body
			continue;
		}else{ // If comparing to foreign body
			//accel+=bods[i].getMass()/(r*r*r);
			accel= VecAdd(accel, VecScaleMulti(VecSub(pos, bods[i].pos), (G*bods[i].mass/(r*r*r))  ) );
		}
	}

	return accel;
}
