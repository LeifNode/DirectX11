#pragma once

#include "ActorComponent.h"
#include "Steering.h"
#include "TransformComponent.h"
#include <map>
#include <vector>
#include <memory>

class BoidManager;

class KinematicComponent : public ActorComponent
{
public:
	KinematicComponent(BoidManager* boidGroup, float maxSpeed = 10.0f, float maxAcceleration = 20.0f);
	KinematicComponent(BoidManager* boidGroup, const XMFLOAT3& initialVelocity,float maxSpeed = 10.0f, float maxAcceleration = 20.0f);
	virtual ~KinematicComponent();

	virtual void VInit(TiXmlElement*);

	virtual const char* VGetName() const { return gName; }

	static const char* gName;

	bool addSteering(Steering*);
	bool removeSteering(Steering*);

	void clear();

	float getSteeringWeight(Steering*);
	bool setSteeringWeight(Steering*, float);

	XMFLOAT3 getVelocity() const { return mVelocity; }
	TransformComponent* getTransform() const { return mpTransform; }

	float getMaxSpeed() const { return mMaxSpeed; }
	void setMaxSpeed(float speed) { mMaxSpeed = speed; }

	float getMaxAcceleration() const { return mMaxAcceleration; }
	void setMaxAcceleration(float acceleration) { mMaxAcceleration = acceleration; }

	void setOrientationFromVelocity();
	void wrapCoordinates();

	BoidManager* getBoidManager() const { return mpBoidManager; }

	virtual void VUpdate(float dt);

private:
	XMFLOAT3 mVelocity;
	float mMaxSpeed;
	float mMaxAcceleration;

	TransformComponent* mpTransform;

	std::vector<Steering*> mSteeringBehaviors;
	BoidManager* mpBoidManager;
};