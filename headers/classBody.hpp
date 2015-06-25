#ifndef _CLASS_BODY_
#define _CLASS_BODY_

#include<string>
#include<vector>
#include <iostream>

template<class T>
class Body{

	private:
		std::string name;
		T mass;
		std::vector<T> pos;
		std::vector<T> vel;
		std::vector<T> posNew;
		std::vector<T> velNew;

	public:
		//Default Constructor
		Body(){
			this->name = "";
			this->mass = 0;

			for(unsigned short i = 0; i < 3; ++i){
				this->pos.push_back(0);
				this->vel.push_back(0);
				this->posNew.push_back(0);
				this->velNew.push_back(0);
			}
		}

		//Constructor
		Body(std::string name, T mass, std::vector<T> pos, std::vector<T> vel){
			this->name = name;
			this->mass = mass;

			if(pos.size() != 3){
				std::cerr << "ERROR: Position vector must be of size 3\n";
				exit(1);
			}

			this->pos = pos;

			if(vel.size() != 3){
				std::cerr << "ERROR: Velocity vector must be of size 3\n";
				exit(1);
			}

			this->vel = vel;

			for(unsigned short i = 0; i < 3; ++i){
				this->posNew.push_back(0);
				this->velNew.push_back(0);
			}
		}

		//Accessors
		std::string getName(){
			return this->name;
		}

		T getMass(){
			return this->mass;
		}

		std::vector<T> getPos(){
			return this->pos;
		}

		std::vector<T> getVel(){
			return this->vel;
		}

		std::vector<T> getPosNew(){
			return this->posNew;
		}

		std::vector<T> getVelNew(){
			return this->velNew;
		}

		//Mutators
		void setName(std::string name){
			this->name = name;
		}

		void setMass(T mass){
			this->mass = mass;
		}

		void setPos(std::vector<T> pos){
			if(pos.size() != 3){
				std::cerr << "ERROR: Position vector must be of size 3\n";
				exit(1);
			}

			for(size_t i = 0; i < pos.size(); ++i){
				this->pos[i] = pos[i];
			}
		}

		void setVel(std::vector<T> vel){
			if(vel.size() != 3){
				std::cerr << "ERROR: Velocity vector must be of size 3\n";
				exit(1);
			}

			for(size_t i = 0; i < vel.size(); ++i){
				this->vel[i] = vel[i];
			}
		}

		void setPosNew(std::vector<T> posNew){
			if(posNew.size() != 3){
				std::cerr << "ERROR: Position vector must be of size 3\n";
				exit(1);
			}

			for(size_t i = 0; i < posNew.size(); ++i){
				this->posNew[i] = posNew[i];
			}
		}

		void setVelNew(std::vector<T> velNew){
			if(velNew.size() != 3){
				std::cerr << "ERROR: Velocity vector must be of size 3\n";
				exit(1);
			}

			for(size_t i = 0; i < velNew.size(); ++i){
				this->velNew[i] = velNew[i];
			}
		}
};

std::vector<Body<double>> bodies;

#endif
