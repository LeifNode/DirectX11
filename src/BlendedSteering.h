#pragma once

#include "Steering.h"
#include "Geometry.h"

class BlendedSteering : public Steering
{
public:
	BlendedSteering();
	virtual ~BlendedSteering();

	bool addSteering(Steering*, float weight = 1.0f);
	bool removeSteering(Steering*);

	void clear();

	float getSteeringWeight(Steering*);
	bool setSteeringWeight(Steering*, float);

	virtual void getSteering(SteeringOut&);

private:
	struct WeightedSteering
	{
		Steering* Steering;
		float Weight;
	};

private:
	std::vector<WeightedSteering> mSteeringBehaviors;
};