#pragma once

#include "d3dApp.h"

class BoidHandler;

class TestApp1 : public D3DApp
{
public:
	TestApp1(HINSTANCE hInstance);
	virtual ~TestApp1();

	void cleanup();

	bool Initialize();

	void hookInputEvents();
	void unhookInputEvents();

	void onKeyDown(IEventDataPtr);

	void onResize();
	void Update(float dt);
	void Draw();

	void onMouseDown(WPARAM btnState, int x, int y);
	void onMouseUp(WPARAM btnState, int x, int y);
	void onMouseMove(WPARAM btnState, int x, int y);

private:
	void buildFX();

private:
	Scene* mpScene;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	ID3D11InputLayout* mInputLayout;
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;

	BoidHandler* mpBoidHandler;

	float mTheta, mPhi, mRadius;

	POINT mLastMousePos;

	float currentRot;
};