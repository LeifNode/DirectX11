#include "AlignSteering.h"
#include "KinematicComponent.h"
#include "TransformComponent.h"
#include "BoidManager.h"

void AlignSteering::getSteering(SteeringOut& out)
{
	unsigned count;
	XMVECTOR targetVelocity = mpKinematic->getBoidManager()->getNeighbourhoodVelocity(mpKinematic, mRadius, mMinDot, &count);

	if (count == 0)
		return;

	XMStoreFloat3(&out.Linear, XMVectorSubtract(targetVelocity, XMLoadFloat3(&mpKinematic->getVelocity())));

	int five = 5;
}