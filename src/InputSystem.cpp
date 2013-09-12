#include "InputSystem.h"
#include <Windows.h>
#include <WindowsX.h>
#include "MouseState.h"
#include "KeyboardState.h"
#include "Events.h"

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem()
{

}

bool InputSystem::initialize()
{
	return true;
}

bool InputSystem::processWindowsMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MouseButton button = MOUSE_NONE;

	switch(msg)
	{
	case WM_MOUSEMOVE:
		mCurrentMouseState.mX = GET_X_LPARAM(lParam);
		mCurrentMouseState.mY = GET_Y_LPARAM(lParam);
		enqueueEvent(IEventDataPtr(new EventData_MouseMove(mCurrentMouseState.mX, mCurrentMouseState.mY)));
		return true;
	case WM_LBUTTONDOWN:
		mCurrentMouseState.mLeft = true;
		enqueueEvent(IEventDataPtr(new EventData_MouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_LEFT)));
		return true;
	case WM_LBUTTONUP:
		mCurrentMouseState.mLeft = false;
		enqueueEvent(IEventDataPtr(new EventData_MouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_LEFT)));
		return true;
	case WM_MBUTTONDOWN:
		mCurrentMouseState.mMiddle = true;
		enqueueEvent(IEventDataPtr(new EventData_MouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_MIDDLE)));
		return true;
	case WM_MBUTTONUP:
		mCurrentMouseState.mMiddle = false;
		enqueueEvent(IEventDataPtr(new EventData_MouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_MIDDLE)));
		return true;
	case WM_RBUTTONDOWN:
		mCurrentMouseState.mRight = true;
		enqueueEvent(IEventDataPtr(new EventData_MouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_RIGHT)));
		return true;
	case WM_RBUTTONUP:
		mCurrentMouseState.mRight = false;
		enqueueEvent(IEventDataPtr(new EventData_MouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MOUSE_RIGHT)));
		return true;
	case WM_XBUTTONDOWN:
		if (GET_XBUTTON_WPARAM(wParam) == 1)
		{
			button = MOUSE_XBUTTON1;
		}
		else if (GET_XBUTTON_WPARAM(wParam) == 2)
		{
			button = MOUSE_XBUTTON2;
		}

		enqueueEvent(IEventDataPtr(new EventData_MouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), button)));
		return true;
		break;
	case WM_XBUTTONUP:
		if (GET_XBUTTON_WPARAM(wParam) == 1)
		{
			button = MOUSE_XBUTTON1;
		}
		else if (GET_XBUTTON_WPARAM(wParam) == 2)
		{
			button = MOUSE_XBUTTON2;
		}

		enqueueEvent(IEventDataPtr(new EventData_MouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), button)));
		return true;
		break;
	case WM_KEYDOWN:
		enqueueEvent(IEventDataPtr(new EventData_KeyboardDown(wParam)));
		return true;
		break;
	case WM_KEYUP:
		enqueueEvent(IEventDataPtr(new EventData_KeyboardUp(wParam)));
		return true;
		break;
	default:
		return false;
	}
}

void InputSystem::enqueueEvent(IEventDataPtr ev)
{
	EventSystem::get()->queueEvent(ev);
}