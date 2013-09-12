#include "MouseState.h"

MouseState::MouseState()
	:mX(0),
	mY(0),
	mLeft(false),
	mMiddle(false),
	mRight(false)
{
}

MouseState::~MouseState()
{}

int MouseState::getX()
{
	return mX;
}

int MouseState::getY()
{
	return mY;
}

bool MouseState::getLeft()
{
	return mLeft;
}

bool MouseState::getMiddle()
{
	return mMiddle;
}

bool MouseState::getRight()
{
	return mRight;
}

int MouseState::getScrollDelta()
{
	return mScroll;
}