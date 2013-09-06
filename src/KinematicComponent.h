#pragma once

#include "ActorComponent.h"

class KinematicComponent : public ActorComponent
{
public:
	KinematicComponent();
	virtual ~KinematicComponent();

	virtual void VUpdate(float dt);

	virtual const char* VGetName() const { return gName; }

	static const char* gName;

	

private:
	XMFLOAT3 mVelocity;

	std::vector<
};