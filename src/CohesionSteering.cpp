#include "CohesionSteering.h"
#include "KinematicComponent.h"
#include "BoidManager.h"
#include "TransformComponent.h"

void CohesionSteering::getSteering(SteeringOut& out)
{
	unsigned count;
	XMVECTOR neighborhoodCenter = mpKinematic->getBoidManager()->getNeighbourhoodCenter(mpKinematic, mRadius, mMinDot, &count);

	if (count == 0)
		return;

	XMVECTOR targetVelocity = XMVectorSubtract(neighborhoodCenter, XMLoadFloat3(&mpKinematic->getTransform()->getPosition()));

	XMStoreFloat3(&out.Linear, targetVelocity);
}