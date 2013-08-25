#include "RotationAnimatorComponent.h"
#include "Actor.h"

const char* RotationAnimatorComponent::gName = "RotationAnimator";

RotationAnimatorComponent::RotationAnimatorComponent(const XMFLOAT3& rotationVector)
	:
	mRotationVector(rotationVector)
{
}

RotationAnimatorComponent::~RotationAnimatorComponent()
{}

void RotationAnimatorComponent::VUpdate(float dt)
{
	if (mpOwnerTransform.expired())
		mpOwnerTransform = mpOwner->getComponent<TransformComponent>(TransformComponent::gName);

	if (!mpOwnerTransform.expired())
		mpOwnerTransform.lock()->rotate(mRotationVector.x * dt, mRotationVector.y * dt, mRotationVector.z * dt);
}