/*
 *	
 */
#pragma once

#include <Windows.h>
#include <queue>
#include "EventSystem.h"

#include "MouseState.h"

class D2DApp;

class InputSystem
{
	friend class D3DApp;
public:
	InputSystem();
	~InputSystem();

	bool initialize();

private:
	bool processWindowsMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	///<summary>
	///Adds an input event that will get dispatched upon update()
	///</summary>
	///<returns></returns>
	void enqueueEvent(IEventDataPtr);

private:
	MouseState mPreviousMouseState;
	MouseState mCurrentMouseState;
};