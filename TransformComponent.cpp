#include "TransformComponent.h"
#include "Scene.h"

const char* TransformComponent::gName = "Transform";

TransformComponent::TransformComponent(ActorPtr pOwner, XMMATRIX* pToWorldInit)
	:ActorComponent(pOwner)
{
	if (pToWorldInit)
		mToWorld = *pToWorldInit;
	else
		mToWorld = XMMatrixIdentity();
}

TransformComponent::~TransformComponent()
{}

void TransformComponent::VPreRender()
{
	Scene::getScene()->pushAndSetMatrix(mToWorld);
}

void TransformComponent::VPostRender()
{
	Scene::getScene()->popMatrix();
}

//XMFLOAT3 TransformComponent::getPosition() const
//{
//	return XMFLOAT3(XMVectorGetX(mToWorld.r[3]), XMVectorGetY(mToWorld.r[3]), XMVectorGetZ(mToWorld.r[3]));
//}
//
//void TransformComponent::setPosition(const XMFLOAT3& newPos)
//{
//	XMVECTOR newVec = XMVectorSet(newPos.x, newPos.y, newPos.z, 1.0f);
//
//	mToWorld.r[3] = newVec;
//}

void TransformComponent::rotate(float x, float y, float z)
{
	mToWorld = XMMatrixRotationRollPitchYaw(x, y, z) * mToWorld;
}