#include "SeparationSteering.h"
#include "KinematicComponent.h"
#include "BoidManager.h"
#include "TransformComponent.h"

void SeparationSteering::getSteering(SteeringOut& out)
{
	unsigned count;
	XMVECTOR neighborhoodCenter = mpKinematic->getBoidManager()->getNeighbourhoodCenter(mpKinematic, mRadius, mMinDot, &count);

	if (count ==  0)
		return;

	XMVECTOR targetVelocity = XMVectorSubtract(XMLoadFloat3(&mpKinematic->getTransform()->getPosition()), neighborhoodCenter);

	//if (XMVectorGetX(XMVector3LengthSq(targetVelocity)) > 0.0f)
		//targetVelocity = XMVectorMultiply(XMVector3Normalize(targetVelocity), XMVectorDivide(XMVectorSet(5.0f, 5.0f, 5.0f, 5.0f), XMVector3LengthSq(targetVelocity)));

	//XMVECTOR maxAcceleration = 

	//if (XMVectorGetX(XMVector3LengthSq(targetVelocity)) > mpKinematic->getMaxAcceleration())
	//	targetVelocity = XMVectorMultiply(XMVector3Normalize(targetVelocity), )

	XMStoreFloat3(&out.Linear, targetVelocity);
}