#pragma once

#include "ActorComponent.h"

class TransformComponent : public ActorComponent
{
	friend class TestApp1;

public:
	TransformComponent(ActorPtr pOwner, XMMATRIX* pToWorldInit = 0);
	virtual ~TransformComponent();

	virtual void VPreRender();
	virtual void VPostRender();

	virtual const char *VGetName() const { return gName; };

	XMMATRIX* getTransform() { return &mToWorld; }

	//XMFLOAT3 getPosition() const;
	//void setPosition(const XMFLOAT3&);

	void rotate(float x, float y, float z);

public:
	static const char* gName;

private:
	XMMATRIX mToWorld;
};