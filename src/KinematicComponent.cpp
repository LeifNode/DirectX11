#include "KinematicComponent.h"
#include "Steering.h"
#include <cassert>

const char* KinematicComponent::gName = "Kinematic";

KinematicComponent::KinematicComponent(float maxSpeed)
	:mMaxSpeed(maxSpeed)
{}

void KinematicComponent::VInit(TiXmlElement* descriptor)
{
	mpTransform = mpOwner->getComponent<TransformComponent>(TransformComponent::gName);

	assert(!mpTransform.expired());
}

bool KinematicComponent::addSteering(Steering* steering, float weight)
{
	for (auto it = mSteeringMap.begin(); it != mSteeringMap.end(); ++it)
	{
		if (it->Steering == steering)
			return false;
	}

	WeightedSteering weightedSteering;
	weightedSteering.Steering = steering;
	weightedSteering.Weight = weight;
	steering->mpKinematic = this;
	mSteeringMap.push_back(weightedSteering);

	return true;
}

bool KinematicComponent::removeSteering(Steering* steering)
{
	for (auto it = mSteeringMap.begin(); it != mSteeringMap.end(); ++it)
	{
		if (it->Steering == steering)
		{
			delete it ->Steering;
			mSteeringMap.erase(it);
			return true;
		}
	}
	
	return false;
}

float KinematicComponent::getSteeringWeight(Steering* steering)
{
	for (auto it = mSteeringMap.begin(); it != mSteeringMap.end(); ++it)
	{
		if (it->Steering == steering)
		{
			return it->Weight;
		}
	}

	return 0.0f;
}

bool KinematicComponent::setSteeringWeight(Steering* steering, float weight)
{
	for (auto it = mSteeringMap.begin(); it != mSteeringMap.end(); ++it)
	{
		if (it->Steering == steering)
		{
			it->Weight = weight;
			return true;
		}
	}

	return false;
}

void KinematicComponent::VUpdate(float dt)
{
	
}