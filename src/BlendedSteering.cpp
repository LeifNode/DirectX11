#include "BlendedSteering.h"
#include "KinematicComponent.h"
#include "TransformComponent.h"
#include "BoidManager.h"
#include <cassert>

BlendedSteering::BlendedSteering()
{
}

BlendedSteering::~BlendedSteering()
{
	clear();
}

bool BlendedSteering::addSteering(Steering* steering, float weight)
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		if (it->Steering == steering)
			return false;
	}

	WeightedSteering weightedSteering;
	weightedSteering.Steering = steering;
	weightedSteering.Weight = weight;

	assert(mpKinematic);
	steering->mpKinematic = mpKinematic;
	mSteeringBehaviors.push_back(weightedSteering);

	return true;
}

bool BlendedSteering::removeSteering(Steering* steering)
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		if (it->Steering == steering)
		{
			delete it ->Steering;
			mSteeringBehaviors.erase(it);
			return true;
		}
	}
	
	return false;
}

void BlendedSteering::clear()
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		delete it->Steering;
		it->Steering = nullptr;
	}

	mSteeringBehaviors.clear();
}

float BlendedSteering::getSteeringWeight(Steering* steering)
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		if (it->Steering == steering)
		{
			return it->Weight;
		}
	}

	return 0.0f;
}

bool BlendedSteering::setSteeringWeight(Steering* steering, float weight)
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		if (it->Steering == steering)
		{
			it->Weight = weight;
			return true;
		}
	}

	return false;
}

void BlendedSteering::getSteering(SteeringOut& steeringOut)
{
	XMVECTOR finalAcceleration = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float totalWeight = 0.0f;
	float maxAcceleration = mpKinematic->getMaxAcceleration();

	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		SteeringOut output = SteeringOut();

		it->Steering->getSteering(output);

		XMVECTOR steeringResult = XMLoadFloat3(&output.Linear);

		//Cap acceleration
		if (XMVectorGetX(XMVector3LengthSq(steeringResult)) > maxAcceleration)
			steeringResult = XMVectorMultiply(XMVector3Normalize(steeringResult), XMVectorSet(maxAcceleration, maxAcceleration, maxAcceleration, maxAcceleration));

		steeringResult = XMVectorMultiply(XMLoadFloat3(&output.Linear), XMVectorSet(it->Weight, it->Weight, it->Weight, it->Weight));
		totalWeight += it->Weight;

		finalAcceleration = XMVectorAdd(finalAcceleration, steeringResult);
	}

	if (totalWeight > 0.0f)
	{
		totalWeight = totalWeight / mSteeringBehaviors.size();
		finalAcceleration = XMVectorMultiply(finalAcceleration, XMVectorSet(totalWeight, totalWeight, totalWeight, totalWeight));

		if (XMVectorGetX(XMVector3LengthSq(finalAcceleration)) > maxAcceleration)
			finalAcceleration = XMVectorMultiply(XMVector3Normalize(finalAcceleration), XMVectorSet(maxAcceleration, maxAcceleration, maxAcceleration, maxAcceleration));
	}

	SteeringOut result;
	XMStoreFloat3(&result.Linear, finalAcceleration);
	steeringOut = result;
}
