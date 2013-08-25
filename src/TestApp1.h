#pragma once

#include "d3dApp.h"

class TestApp1 : public D3DApp
{
public:
	TestApp1(HINSTANCE hInstance);
	~TestApp1();

	bool Initialize();
	void onResize();
	void Update(float dt);
	void Draw();

	void onMouseDown(WPARAM btnState, int x, int y);
	void onMouseUp(WPARAM btnState, int x, int y);
	void onMouseMove(WPARAM btnState, int x, int y);

private:
	void buildGeometryBuffers();
	void buildFX();

private:
	Scene* mpScene;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	ID3D11InputLayout* mInputLayout;
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;

	float mTheta, mPhi, mRadius;

	POINT mLastMousePos;

	float currentRot;
};