#include "BoidManager.h"
#include "KinematicComponent.h"
#include "Geometry.h"

BoidManager::BoidManager()
{
}

BoidManager::~BoidManager()
{
	clear();
}

void BoidManager::clear()
{
	mBoids.clear();
}

std::vector<KinematicComponent*> BoidManager::getNeighborhood(const XMFLOAT3& position, float radius, float minDotProduct)
{
	std::vector<KinematicComponent*> results;

	for (auto it = mBoids.begin(); it != mBoids.end(); ++it)
	{
		Vector3 direction = XMVectorSubtract(XMLoadFloat3(&position), XMLoadFloat3(&(*it)->getTransform().lock()->getPosition()));
		
		if (direction.lengthSquared() > radius * radius)
			continue;
		if (direction.dot((*it)->getVelocity()) > minDotProduct)
			continue;

		results.push_back(*it);
	}

	return results;
}

XMVECTOR BoidManager::getNeighbourhoodCenter(const XMFLOAT3& position, float radius, float minDotProduct, unsigned& countOut)
{
	XMVECTOR totalPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	unsigned count = 0;

	for (auto it = mBoids.begin(); it != mBoids.end(); ++it)
	{
		Vector3 direction = XMVectorSubtract(XMLoadFloat3(&position), XMLoadFloat3(&(*it)->getTransform().lock()->getPosition()));
		
		if (direction.lengthSquared() > radius * radius)
			continue;
		if (direction.dot((*it)->getVelocity()) > minDotProduct)
			continue;

		totalPosition = XMVectorAdd(totalPosition, XMLoadFloat3(&(*it)->getTransform().lock()->getPosition()));
		count++;
	}

	countOut = count;

	if (count == 0)
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	
	float countF = static_cast<float>(count);

	//DirectXMath documentation states that this is a faster operation than XMVectorMultiply if loss in accuracy is okay
	XMVECTOR reciprocal = XMVectorReciprocalEst(XMVectorSet(countF, countF, countF, countF));
	return XMVectorMultiply(totalPosition, reciprocal);
}

XMVECTOR BoidManager::getNeighbourhoodVelocity(const XMFLOAT3& position, float radius, float minDotProduct, unsigned& countOut)
{
	XMVECTOR totalVelocity = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	unsigned count = 0;

	for (auto it = mBoids.begin(); it != mBoids.end(); ++it)
	{
		Vector3 direction = XMVectorSubtract(XMLoadFloat3(&position), XMLoadFloat3(&(*it)->getTransform().lock()->getPosition()));
		
		if (direction.lengthSquared() > radius * radius)
			continue;
		if (direction.dot((*it)->getVelocity()) > minDotProduct)
			continue;

		totalVelocity = XMVectorAdd(totalVelocity, XMLoadFloat3(&(*it)->getVelocity()));
		count++;
	}

	countOut = count;
	
	if (count == 0)
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	float countF = static_cast<float>(count);

	//DirectXMath documentation states that this is a faster operation than XMVectorMultiply if loss in accuracy is okay
	XMVECTOR reciprocal = XMVectorReciprocalEst(XMVectorSet(countF, countF, countF, countF));
	return XMVectorMultiply(totalVelocity, reciprocal);
}

bool BoidManager::addBoid(KinematicComponent* boid)
{
	for (auto it = mBoids.begin(); it != mBoids.end(); ++it)
	{
		if (boid == *it)
			return false;
	}

	mBoids.push_back(boid);
	return true;
}

bool BoidManager::removeBoid(KinematicComponent* boid)
{
	for (auto it = mBoids.begin(); it != mBoids.end(); ++it)
	{
		if (*it == boid)
		{
			mBoids.erase(it);
			return true;
		}
	}

	return false;
}