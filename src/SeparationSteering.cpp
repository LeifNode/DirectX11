#include "SeparationSteering.h"
#include "KinematicComponent.h"
#include "BoidManager.h"
#include "TransformComponent.h"

void SeparationSteering::getSteering(SteeringOut& out)
{
	unsigned count;
	XMVECTOR neighborhoodCenter = mpKinematic->getBoidManager()->getNeighbourhoodCenter(mpKinematic->getTransform().lock()->getPosition(), mRadius, mMinDot, count);

	if (count ==  0)
		return;

	XMVECTOR targetVelocity = XMVectorSubtract(XMLoadFloat3(&mpKinematic->getTransform().lock()->getPosition()), neighborhoodCenter);

	XMStoreFloat3(&out.Linear, targetVelocity);
}