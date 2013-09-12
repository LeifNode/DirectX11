#pragma once

#include "MathHelper.h"

class KinematicComponent;
class Vector3;

struct SteeringOut
{
	SteeringOut():Linear(0.0f, 0.0f, 0.0f){}
	SteeringOut(const XMFLOAT3& linear)
		:Linear(linear)
	{}

	XMFLOAT3 Linear;
};

class Steering
{
	friend class KinematicComponent;

public:
	Steering(){}
	virtual ~Steering(){}

	virtual void getSteering(SteeringOut&){}

protected:
	KinematicComponent* mpKinematic;
};