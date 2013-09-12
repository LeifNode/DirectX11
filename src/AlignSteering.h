#pragma once

#include "BoidSteering.h"

class AlignSteering : public BoidSteering
{
public:
	AlignSteering(float radius, float minDotProduct = -1.0f):BoidSteering(radius, minDotProduct){}
	virtual ~AlignSteering(){}

	virtual void getSteering(SteeringOut& out);
};