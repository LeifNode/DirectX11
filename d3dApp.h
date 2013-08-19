#pragma once

#define WIN32_LEAN_AND_MEAN

#include "d3dStd.h"
#include "GameTimer.h"

class D3DRenderer;

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE appInstance() const { return mhAppInstance; }
	HWND mainWnd() const { return mhWnd; }
	int getClientWidth() const { return mClientWidth; }
	int getClientHeight() const { return mClientHeight; }
	float aspectRatio() const;

	int Run();

	virtual bool Initialize();
	virtual void onResize();
	virtual void Update(float dt)=0;
	virtual void Draw()=0;
	
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void onMouseDown(WPARAM btnState, int x, int y) { }
	virtual void onMouseUp(WPARAM btnState, int x, int y) { }
	virtual void onMouseMove(WPARAM btnState, int x, int y) { }

protected:
	bool initWindow();

	void calculateFrameStats();

protected:

	HINSTANCE mhAppInstance;
	HWND mhWnd;

	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;

	D3DRenderer* mpRenderer;

	GameTimer mTimer;

	std::wstring mMainWndCaption;
	int mClientWidth;
	int mClientHeight;
};
