#include "KinematicComponent.h"

const char* KinematicComponent::gName = "Kinematic";

KinematicComponent::KinematicComponent(float maxSpeed)
	:mMaxSpeed(maxSpeed)
{}

bool KinematicComponent::addSteering(Steering* steering, float weight)
{
	for (auto it = mSteeringMap.begin(); it != mSteeringMap.end(); ++it)
	{
		if (it->Steering == steering)
			return false;
	}

	mSteeringMap.push_back(WeightedSteering(){ Steering = steering; Weight = weight; });
}

bool KinematicComponent::removeSteering(Steering*);

float KinematicComponent::getSteeringWeight(Steering*);
bool KinematicComponent::setSteeringWeight(Steering*, float);