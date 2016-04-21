
#ifndef SPRING_H
#define SPRING_H

#include "Ball.h"

class Spring
{
public:
	//Indices of the balls at either end of the spring
	int ball1;
	int ball2;

	//Tension in the spring
	float tension;

	float springConstant;
	float naturalLength;

	Spring() : ball1(-1), ball2(-1)
	{}
	
	~Spring()
	{}
};

#endif