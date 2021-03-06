#pragma once

#include "Steering.h"
#include "Geometry.h"

class BoidSteering : public Steering
{
	friend class KinematicComponent;

public:
	BoidSteering(float radius, float minDotProduct = -1.0f):mRadius(radius), mMinDot(minDotProduct){}
	virtual ~BoidSteering() {}

	float getRadius() const { return mRadius; }
	float getMinDot() const { return mMinDot; }
	void setRadius(float radius) { mRadius = radius; }
	void setMinDot(float minDot) { mMinDot = minDot; }

protected:
	float mRadius, mMinDot;
};