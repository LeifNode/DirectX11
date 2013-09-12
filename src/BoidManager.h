#pragma once

#include <vector>
#include <list>
#include "MathHelper.h"

class KinematicComponent;

class BoidManager
{
	friend class KinematicComponent;

public:
	BoidManager();
	~BoidManager();

	void clear();
	
	std::vector<KinematicComponent*> getNeighborhood(const XMFLOAT3& position, float radius, float minDotProduct = -1.0f);
	XMVECTOR BoidManager::getNeighbourhoodCenter(const XMFLOAT3& position, float radius, float minDotProduct, unsigned& countOut);
	XMVECTOR BoidManager::getNeighbourhoodVelocity(const XMFLOAT3& position, float radius, float minDotProduct, unsigned& countOut);

private:
	bool addBoid(KinematicComponent* boid);
	bool removeBoid(KinematicComponent* boid);

	std::vector<KinematicComponent*> mBoids;
};