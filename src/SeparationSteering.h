#pragma once

#include "BoidSteering.h"

class SeparationSteering : public BoidSteering
{
public:
	SeparationSteering(float radius, float minDotProduct = -1.0f):BoidSteering(radius, minDotProduct){}
	virtual ~SeparationSteering(){}

	virtual void getSteering(SteeringOut&  out);
};