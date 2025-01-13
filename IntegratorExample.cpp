

#include <iostream>
#include "Integrator.h"

#define NR_OF_DISC_PTS 1500



int main()
{


	auto fun = std::function<float(float)>([=](float x)  -> float {return (x); });
	Integrator <float, float> Integ(fun, NR_OF_DISC_PTS, { 0, 2});
	std::cout<< "Integral value: "<< Integ.integrate() << std::endl;
	 
	
	return 0;
}
