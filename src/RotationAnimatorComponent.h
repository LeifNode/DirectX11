#pragma once

#include "ActorComponent.h"
#include "TransformComponent.h"

class RotationAnimatorComponent : public ActorComponent
{
public:
	RotationAnimatorComponent(const XMFLOAT3& rotationVector);
	virtual ~RotationAnimatorComponent();

	virtual void VUpdate(float dt);

	virtual const char *VGetName() const { return gName; }

	static const char* gName;

private:
	XMFLOAT3 mRotationVector;
	weak_ptr<TransformComponent> mpOwnerTransform;
};