#pragma once

#include "BoidSteering.h"

class CohesionSteering : public BoidSteering
{
public:
	CohesionSteering(float radius, float minDotProduct = -1.0f):BoidSteering(radius, minDotProduct){}
	virtual ~CohesionSteering(){}

	virtual void getSteering(SteeringOut&  out);
};