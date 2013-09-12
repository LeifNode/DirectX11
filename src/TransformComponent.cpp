#include "TransformComponent.h"
#include "Scene.h"
#include "MathHelper.h"

const char* TransformComponent::gName = "Transform";

TransformComponent::TransformComponent(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
	:mPosition(position),
	mRotation(rotation),
	mScale(scale),
	mNeedsRefresh(true)
{
	calculateTransform();
}

TransformComponent::~TransformComponent()
{}

void TransformComponent::VPreRender()
{
	calculateTransform();

	Scene::getScene()->pushAndSetMatrix(mToWorld);
}

void TransformComponent::VPostRender()
{
	Scene::getScene()->popMatrix();
}

const XMMATRIX* TransformComponent::getToWorldLocal()
{
	calculateTransform();
	return &mToWorld;
}

XMMATRIX TransformComponent::getFromWorldLocal()
{
	calculateTransform();
	return XMMatrixInverse(nullptr, mToWorld);
}

XMMATRIX TransformComponent::getToWorldGlobal()
{
	WeakActorPtr currentParent = mpOwner->getParent();

	XMMATRIX toWorld = mToWorld;
	std::weak_ptr<TransformComponent> currentTransform;

	while (!currentParent.expired())
	{
		currentTransform = currentParent.lock()->getComponent<TransformComponent>(TransformComponent::gName);

		if (!currentTransform.expired())
		{
			toWorld = *currentTransform.lock()->getToWorldLocal() * toWorld;
		}

		currentParent = currentParent.lock()->getParent();
	}

	return toWorld;
}

XMMATRIX TransformComponent::getFromWorldGlobal()
{
	return XMMatrixInverse(nullptr, getToWorldGlobal());
}

void TransformComponent::rotate(float x, float y, float z)
{
	mRotation = XMFLOAT3(MathHelper::WrapAngle(mRotation.x + x), MathHelper::WrapAngle(mRotation.y + y), MathHelper::WrapAngle(mRotation.z + z));
	flagRefresh();
}

void TransformComponent::calculateTransform()
{
	if (mNeedsRefresh)
	{
		Vector3 zeroFloat = Vector3();

		mToWorld = XMMatrixAffineTransformation(XMLoadFloat3(&mScale), 
												XMLoadFloat3(&zeroFloat), 
												XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation)), 
												XMLoadFloat3(&mPosition));

		mNeedsRefresh = false;
	}
}