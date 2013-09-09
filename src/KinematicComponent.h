#pragma once

#include "ActorComponent.h"
#include "Steering.h"
#include <map>
#include <vector>

class KinematicComponent : public ActorComponent
{
public:
	KinematicComponent(float maxSpeed = 10.0f);
	virtual ~KinematicComponent() { }

	virtual const char* VGetName() const { return gName; }

	static const char* gName;

	bool addSteering(Steering*, float weight = 1.0f);
	bool removeSteering(Steering*);

	float getSteeringWeight(Steering*);
	bool setSteeringWeight(Steering*, float);

	XMFLOAT3 getVelocity() const { return mVelocity; }

	float getMaxSpeed() const { return mMaxSpeed; }
	void setMaxSpeed(float speed) { mMaxSpeed = speed; }

	virtual void VUpdate(float dt);
	
private:
	struct WeightedSteering
	{
		Steering* Steering;
		float Weight;
	};

private:
	XMFLOAT3 mVelocity;
	float mMaxSpeed;

	std::vector<WeightedSteering> mSteeringMap;
};