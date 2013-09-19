#include "KinematicComponent.h"
#include "Steering.h"
#include "BoidManager.h"
#include <cassert>

const char* KinematicComponent::gName = "Kinematic";

KinematicComponent::KinematicComponent(BoidManager* boidManager, float maxSpeed, float maxAcceleration)
	:mMaxSpeed(maxSpeed),
	mMaxAcceleration(maxAcceleration),
	mpBoidManager(boidManager),
	mVelocity(0.0f, 0.0f, 0.0f)
{
	assert(mpBoidManager);
	mpBoidManager->addBoid(this);
}

KinematicComponent::KinematicComponent(BoidManager* boidManager, const XMFLOAT3& initialVelocity, float maxSpeed, float maxAcceleration)
	:mMaxSpeed(maxSpeed),
	mMaxAcceleration(maxAcceleration),
	mpBoidManager(boidManager),
	mVelocity(initialVelocity)
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
	mpTransform = mpOwner->getComponent<TransformComponent>(TransformComponent::gName).lock().get();

	assert(!mpTransform.expired());
}

bool KinematicComponent::addSteering(Steering* steering)
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		if ((*it) == steering)
			return false;
	}

	steering->mpKinematic = this;
	mSteeringBehaviors.push_back(steering);

	return true;
}

bool KinematicComponent::removeSteering(Steering* steering)
{
	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		if ((*it) == steering)
		{
			delete (*it);
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
		delete (*it);
		(*it) = nullptr;
	}

	mSteeringBehaviors.clear();
}

void KinematicComponent::setOrientationFromVelocity()
{
	XMVECTOR rotationVec = XMVector3Normalize(XMLoadFloat3(&mVelocity));

	mpTransform->setRotation(XMVectorGetX(rotationVec), XMVectorGetY(rotationVec), XMVectorGetZ(rotationVec));
}

//The weighted kinematic functionality should be pushed into its own steering behavior
void KinematicComponent::VUpdate(float dt)
{
	XMVECTOR finalAcceleration = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	for (auto it = mSteeringBehaviors.begin(); it != mSteeringBehaviors.end(); ++it)
	{
		SteeringOut output = SteeringOut();

		(*it)->getSteering(output);

		XMVECTOR steeringResult = XMLoadFloat3(&output.Linear);

		//Cap acceleration
		if (XMVectorGetX(XMVector3LengthSq(steeringResult)) > mMaxAcceleration)
			steeringResult = XMVectorMultiply(XMVector3Normalize(steeringResult), XMVectorSet(mMaxAcceleration, mMaxAcceleration, mMaxAcceleration, mMaxAcceleration));


		finalAcceleration = XMVectorAdd(finalAcceleration, steeringResult);
	}

	float size = (float)mSteeringBehaviors.size();
	finalAcceleration = XMVectorDivide(finalAcceleration, XMVectorSet(size, size, size, size));

	//Integration

	XMVECTOR currentPos = XMLoadFloat3(&mpTransform->getPosition());
	XMVECTOR vel = XMLoadFloat3(&mVelocity);
	currentPos = XMVectorMultiplyAdd(vel, XMVectorSet(dt, dt, dt, dt), currentPos);

	float drag = powf(0.9f, dt);

	mpTransform->setPosition(XMVectorGetX(currentPos), XMVectorGetY(currentPos), XMVectorGetZ(currentPos));

	vel = XMVectorMultiply(vel, XMVectorSet(drag, drag, drag, drag)); //Adds drag to current velocity
	vel = XMVectorMultiplyAdd(finalAcceleration, XMVectorSet(dt, dt, dt, dt), vel); //Adds acceleration to current velocity

	//Cap velocity
	if (XMVectorGetX(XMVector3LengthSq(vel)) > mMaxSpeed * mMaxSpeed)
		vel = XMVectorMultiply(XMVector3Normalize(vel), XMVectorSet(mMaxSpeed, mMaxSpeed, mMaxSpeed, mMaxSpeed));

	XMStoreFloat3(&mVelocity, vel); //Save velocity

	setOrientationFromVelocity();
	wrapCoordinates();
}

void KinematicComponent::wrapCoordinates()
{
	XMFLOAT3 position = mpTransform->getPosition();

	const float BOX_BOUNDS = 50.0f;

	if (position.x > BOX_BOUNDS)
		position.x = -BOX_BOUNDS;
	else if (position.x < -BOX_BOUNDS)
		position.x = BOX_BOUNDS;

	if (position.y > BOX_BOUNDS)
		position.y = -BOX_BOUNDS;
	else if (position.y < -BOX_BOUNDS)
		position.y = BOX_BOUNDS;

	if (position.z > BOX_BOUNDS)
		position.z = -BOX_BOUNDS;
	else if (position.z < -BOX_BOUNDS)
		position.z = BOX_BOUNDS;

	mpTransform->setPosition(position);
}