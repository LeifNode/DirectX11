#pragma once

#include <vector>
#include "MathHelper.h"

class KinematicComponent;

class BoidManager
{
	friend class KinematicComponent;

public:
	BoidManager();
	~BoidManager();

	void clear();
	
	std::vector<KinematicComponent*> getNeighborhood(KinematicComponent* kinematic, float radius, float minDotProduct = -1.0f);
	XMVECTOR BoidManager::getNeighbourhoodCenter(KinematicComponent* kinematic, float radius, float minDotProduct, unsigned* countOut);
	XMVECTOR BoidManager::getNeighbourhoodVelocity(KinematicComponent* kinematic, float radius, float minDotProduct, unsigned* countOut);

	KinematicComponent* getLastBoid() { return (*mBoids.rbegin()); }

private:
	bool addBoid(KinematicComponent* boid);
	bool removeBoid(KinematicComponent* boid);

	std::vector<KinematicComponent*> mBoids;
};