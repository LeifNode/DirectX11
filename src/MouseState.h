#pragma once

class InputSystem;

#include <string>

enum MouseButton
{
	MOUSE_NONE,
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,
	MOUSE_XBUTTON1,
	MOUSE_XBUTTON2,
};

const std::string MOUSE_BUTTON_NAMES[6] = 
{
	"MOUSE_NONE",
	"MOUSE_LEFT",
	"MOUSE_MIDDLE",
	"MOUSE_RIGHT",
	"MOUSE_XBUTTON1",
	"MOUSE_XBUTTON2",
};


class MouseState
{
	friend class InputSystem;
public:
	MouseState();
	~MouseState();

	int getX();
	int getY();
	bool getLeft();
	bool getMiddle();
	bool getRight();
	int getScrollDelta();
private:
	int mX, mY;
	bool mLeft, mMiddle, mRight;
	int mScroll;
};