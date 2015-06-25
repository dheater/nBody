#ifndef _VECMANIP_
#define _VECMANIP_

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

template<typename T> T mag(std::vector<T> vec){

	T sum(0);

	for(size_t i = 0; i<vec.size(); ++i) sum+=(vec[i]*vec[i]);

	return sqrt(sum);

}

template<typename T> T toUnitVec(std::vector<T> vec){
  std::vector<T> uVec(vec.size());

  for(size_t i = 0; i < vec.size(); ++i) uVec[i] = vec[i] / mag(vec);

  return uVec;
}

template<typename T> std::vector<T> VecScaleMulti(std::vector<T> vec, T sca){
	std::vector<T> ret;
	for(size_t i = 0; i < vec.size(); ++i){
		ret.push_back(vec[i]*sca);
	}
	return ret;
}

template<typename T> std::vector<T> VecAdd(std::vector<T> vec1, std::vector<T> vec2){
	std::vector<T> ret;
	if(vec1.size() != vec2.size()){
		std::cerr << "\n\t In std::vector<double> VecAdd: vec1 and vec2 are of different size. Cannot add...\n\t\tReturning zero vector of size vec1...\n";

		for(size_t i = 0; i<vec1.size();++i) ret.push_back(0);

		return ret;
	} else {

		for(size_t i = 0; i < vec1.size(); ++i) ret.push_back(vec1[i]+vec2[i]);

	}
	return ret;
}

template<typename T> std::vector<T> VecSub(std::vector<T> vec1, std::vector<T> vec2){
	std::vector<T> ret;
	if(vec1.size() != vec2.size()){
		std::cerr << "\n\t In std::vector<double> VecSub: vec1 and vec2 are of different size. Cannot subtract...\n\t\tReturning zero vector of size vec1...\n";

		for(size_t i = 0; i<vec1.size();++i) ret.push_back(0);

		return ret;
	} else {

		for(size_t i = 0; i < vec1.size(); ++i) ret.push_back(vec1[i]-vec2[i]);

	}
	return ret;
}

template<typename T> std::vector<T> XProd(std::vector<T> vec1, std::vector<T> vec2){
	std::vector<T> ret;
	if(vec1.size() != vec2.size() && vec1.size() + vec2.size() != 6){
		std::cerr << "/n/t In std::vector<double> VecAdd: vec1 and vec2 are of different size. Cannot add...\nReturning zero vector of size vec1...\n";
	}

	ret.push_back(vec1[1]*vec2[2] - vec1[2]*vec2[1]);
	ret.push_back(vec1[2]*vec2[0] - vec1[0]*vec2[2]);
	ret.push_back(vec1[0]*vec2[1] - vec1[1]*vec2[0]);

	return ret;
}

template<typename T> void zeroVec(std::vector<T> &vec){
	for(size_t i=0;i<vec.size();++i){
		vec[i]=0;
	}
}

template<typename T> std::vector<T> printVec(std::vector<T> vec){
	for(size_t i=0;i<vec.size();++i){
		std::cout << vec[i] << '\t';
	}
	std::cout << std::endl;
	return vec;
}
#endif
