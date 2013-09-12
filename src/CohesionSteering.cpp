#include "CohesionSteering.h"
#include "KinematicComponent.h"
#include "BoidManager.h"
#include "TransformComponent.h"

void CohesionSteering::getSteering(SteeringOut& out)
{
	unsigned count;
	XMVECTOR neighborhoodCenter = mpKinematic->getBoidManager()->getNeighbourhoodCenter(mpKinematic->getTransform().lock()->getPosition(), mRadius, mMinDot, count);

	if (count ==  0)
		return;

	XMVECTOR targetVelocity = XMVectorSubtract(neighborhoodCenter, XMLoadFloat3(&mpKinematic->getTransform().lock()->getPosition()));

	XMStoreFloat3(&out.Linear, targetVelocity);
}