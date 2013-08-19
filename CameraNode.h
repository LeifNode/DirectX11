#pragma once

#include <DirectXMath.h>

using DirectX::XMMATRIX;

class TestApp1;

class CameraNode
{
	friend class TestApp1;

public:
	CameraNode() { mView = mProj = DirectX::XMMatrixIdentity(); }
	~CameraNode() { }

	const XMMATRIX* getView() const { return &mView; }
	const XMMATRIX* getProjection() const { return &mProj; }
	XMMATRIX getViewProj() { return mView * mProj; }

private:
	void setView(XMMATRIX& mat) { mView = mat; }
	void setProj(XMMATRIX& mat) { mProj = mat; }
 
	XMMATRIX mView;
	XMMATRIX mProj;
};