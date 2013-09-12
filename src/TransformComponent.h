#pragma once

#include "ActorComponent.h"

class TransformComponent : public ActorComponent
{
	friend class TestApp1;

public:
	TransformComponent(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);
	virtual ~TransformComponent();

	virtual void VPreRender();
	virtual void VPostRender();

	virtual const char *VGetName() const { return gName; };

	//Gets the transform of this object reletive to its parent.
	const XMMATRIX* getToWorldLocal();
	//Returns the inverse of the to world transform. 
	XMMATRIX getFromWorldLocal();

	//Gets the transform of this object concatinated with all of its parent's to world transforms.
	XMMATRIX getToWorldGlobal();
	//Gets the inverse of the to world global transform.
	XMMATRIX getFromWorldGlobal();


	const XMFLOAT3& getPosition() const { return mPosition; }
	void setPosition(const XMFLOAT3& position) { mPosition = position; flagRefresh(); }
	void setPosition(float x, float y, float z) { mPosition = XMFLOAT3(x, y, z); flagRefresh(); }
	_declspec(property(get=getX, put=setX)) float X;
	_declspec(property(get=getY, put=setY)) float Y;
	_declspec(property(get=getZ, put=setZ)) float Z;
	float getX() const { return mPosition.x; }
	float getY() const { return mPosition.y; }
	float getZ() const { return mPosition.z; }
	void setX(float x) { mPosition.x = x; flagRefresh(); }
	void setY(float y) { mPosition.y = y; flagRefresh(); }
	void setZ(float z) { mPosition.z = z; flagRefresh(); }

	const XMFLOAT3& getRotation() const { return mRotation; }
	void setRotation(const XMFLOAT3& rotation) { mRotation = rotation; flagRefresh(); }
	void setRotation(float x, float y, float z) { mRotation = XMFLOAT3(x, y, z); flagRefresh(); }
	_declspec(property(get=getXRotation, put=setXRotation)) float XRotation;
	_declspec(property(get=getYRotation, put=setYRotation)) float YRotation;
	_declspec(property(get=getZRotation, put=setZRotation)) float ZRotation;
	float getXRotation() const { return mRotation.x; }
	float getYRotation() const { return mRotation.y; }
	float getZRotation() const { return mRotation.z; }
	void setXRotation(float x) { mRotation.x = x; flagRefresh(); }
	void setYRotation(float y) { mRotation.y = y; flagRefresh(); }
	void setZRotation(float z) { mRotation.z = z; flagRefresh(); }

	const XMFLOAT3& getScale() const { return mScale; }
	void setScale(const XMFLOAT3& rotation) { mScale = rotation; flagRefresh(); }
	void setScale(float x, float y, float z) { mScale = XMFLOAT3(x, y, z); flagRefresh(); }
	_declspec(property(get=getXScale, put=setXScale)) float XScale;
	_declspec(property(get=getYScale, put=setYScale)) float YScale;
	_declspec(property(get=getZScale, put=setZScale)) float ZScale;
	float getXScale() const { return mScale.x; }
	float getYScale() const { return mScale.y; }
	float getZScale() const { return mScale.z; }
	void setXScale(float x) { mScale.x = x; flagRefresh(); }
	void setYScale(float y) { mScale.y = y; flagRefresh(); }
	void setZScale(float z) { mScale.z = z; flagRefresh(); }

	void rotate(float x, float y, float z);

public:
	static const char* gName;

private:
	void flagRefresh() { mNeedsRefresh = true; }
	void calculateTransform();

private:
	XMMATRIX mToWorld;

	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

	bool mNeedsRefresh;
};