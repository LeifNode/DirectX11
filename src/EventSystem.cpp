#include "EventSystem.h"

EventSystem* EventSystem::mpInstance = NULL;

EventSystem::EventSystem()
{}

EventSystem::~EventSystem()
{
	mpInstance = NULL;
	clear();
}

EventSystem* EventSystem::get()
{
	if (mpInstance)
		return mpInstance;
	else
		return mpInstance = new EventSystem();
}

bool EventSystem::addListener(const EventType& type, const EventListenerDelegate& eventDelegate)
{
	EventListenerList& eventListenerList = mEventListeners[type];

	for (auto i = eventListenerList.begin(); i != eventListenerList.end(); ++i)
	{
		if (eventDelegate == (*i))
			return false;
	}

	eventListenerList.push_back(eventDelegate);

	return true;
}

bool EventSystem::removeListener(const EventType& type, const EventListenerDelegate& eventDelegate)
{
	auto it = mEventListeners.find(type);

	if (it != mEventListeners.end())
	{
		EventListenerList& listeners = it->second;

		for (auto i = listeners.begin(); i != listeners.end(); ++i)
		{
			if (eventDelegate == (*i))
			{
				listeners.erase(i);
				return true;
			}
		}
	}

	return false;
}

bool EventSystem::triggerEvent(const IEventDataPtr& pEvent) const
{
	bool processed = false;

	auto i = mEventListeners.find(pEvent->getEventType());
	if (i != mEventListeners.end())
	{
		const EventListenerList& eventListenerList = i->second;
		for (EventListenerList::const_iterator it = eventListenerList.cbegin(); it != eventListenerList.cend(); ++it)
		{
			EventListenerDelegate listener = (*it);
			listener(pEvent);
			processed = true;
		}
	}

	return processed;
}

bool EventSystem::queueEvent(const IEventDataPtr& pEvent)
{
	if (!pEvent)
		return false;

	auto i = mEventListeners.find(pEvent->getEventType());
	if (i != mEventListeners.end())
	{
		mQueue.push_back(pEvent);
		return true;
	}
	else
	{
		//May cause errors down the road
		return false;
	}
}

void EventSystem::clear()
{
	for (auto i = mEventListeners.begin(); i != mEventListeners.end(); ++i)
	{
		i->second.clear();
	}

	mEventListeners.clear();
}

//Cut the max time for now
bool EventSystem::update(unsigned long maxMillis)
{
	while (!mQueue.empty())
	{
		IEventDataPtr pEvent = mQueue.front();
		mQueue.pop_front();

		const EventType& eventType = pEvent->getEventType();

		auto find = mEventListeners.find(eventType);
		if (find != mEventListeners.end())
		{
			const EventListenerList& eventListeners = find->second;

			for (auto i = eventListeners.begin(); i != eventListeners.end(); ++i)
			{
				EventListenerDelegate listener = (*i);
				listener(pEvent);
			}
		}
	}

	return true;
}