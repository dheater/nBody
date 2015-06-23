#ifndef _RK4_
#define _RK4_

#include <vector>
#include <iostream>
#include <stdlib.h>

template<typename T, typename F> void evaluate(
	F derivFunc,
	std::vector<T> &posIn,
	std::vector<T> &velIn,
	T t,
	T dt,
	std::vector<T> &posOut,
	std::vector<T> &velOut)
	{
		
		if(posIn.size() !=3 || velIn.size() != 3){
			std::cerr << "\n\tERROR: Size of state vector must be 3\n\tTerminating Processes...\n\n";
			std::exit(EXIT_FAILURE);
		}
		
		T halfDt = dt * (T)(0.5);
		T tMid = t + halfDt;
		std::vector<T> dVelIn = derivFunc(posIn, velIn, t);
		std::vector<T> velB = VecAdd(velIn, VecScaleMulti(dVelIn, halfDt)); // velB = velIn + dVelIn * halfDt;
		std::vector<T> posB = VecAdd(posIn, VecScaleMulti(velIn, halfDt)); //posIn + velIn * halfDt;
		std::vector<T> dVelB = derivFunc(posB, velB, tMid);
		std::vector<T> velC = VecAdd(velB, VecScaleMulti(dVelB, halfDt)); // velB + dVelB * halfDt;
		std::vector<T> posC = VecAdd(posB, VecScaleMulti(velB,halfDt)); // posB + velB * halfDt;
		std::vector<T> dVelC = derivFunc(posC, velC, tMid);
		std::vector<T> velD = VecAdd(velC, VecScaleMulti(dVelC, dt)); // velC + dVelC * dt;
		std::vector<T> posD = VecAdd(posC, VecScaleMulti(velC, dt)); // posC + velC * dt;
		std::vector<T> dVelD = derivFunc(posD, velD, t + dt);
		
		//velOut = velIn + dt * T(1.0 / 6.0) * (dVelIn + T(2.0) * (dVelB + dVelC) + dVelD);
		velOut = VecAdd(velIn, VecScaleMulti(VecScaleMulti(VecAdd(dVelD, VecAdd(dVelIn, VecScaleMulti(VecAdd(dVelB, dVelC), (T)(2.0)))),(T)(1.0/6.0)), dt));
		
		//posOut = posIn + dt * T(1.0 / 6.0) * (velIn + T(2.0) * (velB + velC) + velD);
		posOut = VecAdd(posIn, VecScaleMulti(VecScaleMulti(VecAdd(velD, VecAdd(velIn, VecScaleMulti(VecAdd(velB, velC),(T)(2.0)))), (T)(1.0/6.0)), dt));
	}
	
#endif