#include <iostream>
#include "headers/classBody.hpp"
#include "headers/vecManip.hpp"

int main(void){
	
	//Body Init=================+
	
	//Earth ============NAME======MASS======POS========VEL======+
	Body<double> earth("Earth", 5.97e24, {0, 0, 0}, {0, 0, 0});
	bodies.push_back(earth);
	
	//Satellite =======NAME====MASS======POS==========VEL=======+
	Body<double> sat("Benley", 5000, {7e6, 0, 0}, {0, 7545.687, 0});
	bodies.push_back(sat);
	
	//End Body Init=============+

	for(size_t j=0; j<bodies.size();++j){
	
		std::cout << "Name: " << bodies[j].getName() << "\nMass: " << bodies[j].getMass() << "\nPos: ";
		
		for(size_t i = 0; i < bodies[j].getPos().size(); ++i){
			std::cout << bodies[j].getPos()[i] << ' ';
		}
		
		std::cout << "\nVel: ";
		
		for(size_t i = 0; i < bodies[j].getVel().size(); ++i){
			std::cout << bodies[j].getVel()[i] << ' ';
		}
		
		std::cout << "\n\n";
	}
	
	earth.setPosNew({10,20,35});
	
	for(int i(0);i<3;++i){
		std::cout << earth.getPosNew()[i] << ' ';
	}
	
	std::cout << "\n\n";
	
	
	
	
	return 0;
}