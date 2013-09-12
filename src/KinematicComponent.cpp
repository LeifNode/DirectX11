#include "KinematicComponent.h"
#include "Steering.h"
#include "BoidManager.h"
#include <cassert>

const char* KinematicComponent::gName = "Kinematic";

KinematicComponent::KinematicComponent(BoidManager* boidManager, float maxSpeed, float maxAcceleration)
	:mMaxSpeed(maxSpeed),
	mMaxAcceleration(maxAcceleration),
	mpBoidManager(boidManager)
{
	assert(mpBoidManager);
	mpBoidManager->addBoid(this);
}

KinematicComponent::~KinematicComponent()
{
	mpBoidManager->removeBoid(this);
	clear();
}

void KinematicComponent::VInit(TiXmlElement* descriptor)
{
	mpTransform = mpOwner->getComponent<TransformComponent>(TransformComponent::gName);

	assert(!mpTransform.expired());
}

bool KinematicComponent::addSteering(Steering* steering, float weight)
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		if (it->Steering == steering)
			return false;
	}

	WeightedSteering weightedSteering;
	weightedSteering.Steering = steering;
	weightedSteering.Weight = weight;
	steering->mpKinematic = this;
	mSteeringBehaviors.push_back(weightedSteering);

	return true;
}

bool KinematicComponent::removeSteering(Steering* steering)
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

void KinematicComponent::clear()
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		delete it->Steering;
		it->Steering = nullptr;
	}

	mSteeringBehaviors.clear();
}

float KinematicComponent::getSteeringWeight(Steering* steering)
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

bool KinematicComponent::setSteeringWeight(Steering* steering, float weight)
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

void KinematicComponent::setOrientationFromVelocity()
{
	XMVECTOR rotationVec = XMVector3Normalize(XMLoadFloat3(&mVelocity));

	mpTransform.lock()->setRotation(XMVectorGetX(rotationVec) * XM_2PI, XMVectorGetY(rotationVec) * XM_2PI, XMVectorGetZ(rotationVec) * XM_2PI);
}

//The weighted kinematic functionality should be pushed into its own steering behavior
void KinematicComponent::VUpdate(float dt)
{
	XMVECTOR finalAcceleration = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float totalWeight = 0.0f;

	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		SteeringOut output = SteeringOut();

		it->Steering->getSteering(output);

		XMVECTOR steeringResult = XMLoadFloat3(&output.Linear);

		if (XMVectorGetX(XMVector3LengthSq(steeringResult)) > mMaxAcceleration)
			steeringResult = XMVectorMultiply(XMVector3Normalize(steeringResult), XMVectorSet(mMaxAcceleration, mMaxAcceleration, mMaxAcceleration, mMaxAcceleration));

		steeringResult = XMVectorMultiply(XMLoadFloat3(&output.Linear), XMVectorSet(it->Weight, it->Weight, it->Weight, it->Weight));
		totalWeight += it->Weight;

		finalAcceleration = XMVectorAdd(finalAcceleration, steeringResult);
	}

	if (totalWeight > 0.0f)
	{
		totalWeight = 1.0f / totalWeight;
		finalAcceleration = XMVectorMultiply(finalAcceleration, XMVectorSet(totalWeight, totalWeight, totalWeight, totalWeight));
	}

	//Integration
	float drag = powf(0.7, dt);

	XMVECTOR vel = XMVectorMultiply(XMLoadFloat3(&mVelocity), XMVectorSet(drag, drag, drag, drag)); //Adds drag to current velocity
	vel = XMVectorMultiplyAdd(finalAcceleration, XMVectorSet(dt, dt, dt, dt), vel); //Adds acceleration to current velocity

	//Cap velocity
	if (XMVectorGetX(XMVector3LengthSq(vel)) > mMaxSpeed * mMaxSpeed)
		vel = XMVectorMultiplyAdd(XMVector3Length(vel), XMVectorSet(mMaxSpeed, mMaxSpeed, mMaxSpeed, mMaxSpeed), vel);

	XMStoreFloat3(&mVelocity, vel); //Save velocity

	XMVECTOR currentPos = XMLoadFloat3(&mpTransform.lock()->getPosition());
	currentPos = XMVectorMultiplyAdd(vel, XMVectorSet(dt, dt, dt, dt), currentPos);

	mpTransform.lock()->setPosition(XMVectorGetX(currentPos), XMVectorGetY(currentPos), XMVectorGetZ(currentPos));

	setOrientationFromVelocity();
}