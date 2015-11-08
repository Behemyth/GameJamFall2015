#include "Rand.h"

std::mt19937 generator = std::mt19937(uint(time(NULL)));

//Uniform Random Floating Point Number
float GetDistribution(std::uniform_real_distribution<float> distro) {
		
	return distro(generator);
}

//Uniform Random Integer
int GetDistribution(std::uniform_int_distribution<int> distro) {
	return distro(generator);
}

//Normal Random Floating Point Number
float GetDistribution(std::normal_distribution<float> distro) {
	return distro(generator);
}