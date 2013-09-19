#pragma once

#include "MathHelper.h"

class KinematicComponent;
class BlendedSteering;
class Vector3;

struct SteeringOut
{
	SteeringOut():Linear(0.0f, 0.0f, 0.0f){}
	SteeringOut(const SteeringOut& other):Linear(other.Linear) {}
	SteeringOut(const XMFLOAT3& linear)
		:Linear(linear)
	{}

	XMFLOAT3 Linear;
};

class Steering
{
	friend class KinematicComponent;
	friend class BlendedSteering;

public:
	Steering(){}
	virtual ~Steering(){}

	virtual void getSteering(SteeringOut&){}

protected:
	KinematicComponent* mpKinematic;
};