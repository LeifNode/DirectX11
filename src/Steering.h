#pragma once

class KinematicComponent;

class SteeringOut
{
	XMFLOAT3 LinearVelocity;
};

class Steering
{
	friend class KinematicComponent;

public:
	Steering();
	virtual ~Steering();

	virtual void getSteering(SteeringOut&);

private:
	KinematicComponent* mpKinematic;
};