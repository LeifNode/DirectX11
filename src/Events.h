#pragma once

#include "EventSystem.h"
#include "MouseState.h"
#include "KeyboardState.h"

//Mouse events
class EventData_MouseDown : public BaseEventData
{
public:
	EventData_MouseDown(int mouseX, int mouseY, MouseButton mouseButton) : x(mouseX), y(mouseY), mButton(mouseButton) { }
	virtual ~EventData_MouseDown() { }

	virtual const EventType& getEventType() const { return skEventType; }
	virtual const char* getName() const { return "EventData_MouseDown"; }
	virtual IEventDataPtr copy() const
	{
		return IEventDataPtr(new EventData_MouseDown(x, y, mButton));
	}

	int getX() const { return x; }
	int getY() const { return y; }
	MouseButton getButton() const { return mButton; }

public:
	static const EventType skEventType;

private:
	int x, y;
	MouseButton mButton;
};

class EventData_MouseUp : public BaseEventData
{
public:
	EventData_MouseUp(int mouseX, int mouseY, MouseButton mouseButton) : x(mouseX), y(mouseY), mButton(mouseButton) { }
	virtual ~EventData_MouseUp() { }

	virtual const EventType& getEventType() const { return skEventType; }
	virtual const char* getName() const { return "EventData_MouseUp"; }
	virtual IEventDataPtr copy() const
	{
		return IEventDataPtr(new EventData_MouseUp(x, y, mButton));
	}

	int getX() const { return x; }
	int getY() const { return y; }
	MouseButton getButton() const { return mButton; }

public:
	static const EventType skEventType;

private:
	int x, y;
	MouseButton mButton;
};

class EventData_MouseMove : public BaseEventData
{
public:
	EventData_MouseMove(int mouseX, int mouseY) : x(mouseX), y(mouseY) { }
	virtual ~EventData_MouseMove() { }

	virtual const EventType& getEventType() const { return skEventType; }
	virtual const char* getName() const { return "EventData_MouseUp"; }
	virtual IEventDataPtr copy() const
	{
		return IEventDataPtr(new EventData_MouseMove(x, y));
	}

	int getX() const { return x; }
	int getY() const { return y; }

public:
	static const EventType skEventType;

private:
	int x, y;
};

//Keyboard events
class EventData_KeyboardDown : public BaseEventData
{
public:
	EventData_KeyboardDown(KeyboardKey key) : key(key) { }
	EventData_KeyboardDown(WPARAM key) : key((KeyboardKey)key) { }
	virtual ~EventData_KeyboardDown() { }

	virtual const EventType& getEventType() const { return skEventType; }
	virtual const char* getName() const { return "EventData_KeyboardDown"; }
	virtual IEventDataPtr copy() const
	{
		return IEventDataPtr(new EventData_KeyboardDown(key));
	}

	KeyboardKey getKey() const { return key; }

public:
	static const EventType skEventType;

private:
	KeyboardKey key;
};

class EventData_KeyboardUp : public BaseEventData
{
public:
	EventData_KeyboardUp(KeyboardKey key) : key(key) { }
	EventData_KeyboardUp(WPARAM key) : key((KeyboardKey)key) { }
	virtual ~EventData_KeyboardUp() { }

	virtual const EventType& getEventType() const { return skEventType; }
	virtual const char* getName() const { return "EventData_KeyboardUp"; }
	virtual IEventDataPtr copy() const
	{
		return IEventDataPtr(new EventData_KeyboardUp(key));
	}

	KeyboardKey getKey() const { return key; }

public:
	static const EventType skEventType;

private:
	KeyboardKey key;
};