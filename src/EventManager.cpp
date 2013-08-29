#include "EventManager.h"

EventManager* EventManager::mpInstance = NULL;
GenericObjectFactory<IEventData, EventType> gEventFactory;

EventManager::EventManager()
{}

EventManager::~EventManager()
{
	mpInstance = NULL;
}

EventManager* EventManager::get()
{
	if (mpInstance)
		return mpInstance;
	else
		return mpInstance = new EventManager();
}

bool EventManager::addListener(const EventType& type, const EventListenerDelegate& eventDelegate)
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

bool EventManager::triggerEvent(const IEventDataPtr& pEvent) const
{
	bool processed = false;

	auto i = mEventListeners.find(pEvent->VGetEventType());
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

bool EventManager::queueEvent(const IEventDataPtr& pEvent)
{
	if (!pEvent)
		return false;

	auto i = mEventListeners.find(pEvent->VGetEventType());
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

//Cut the max time for now
bool EventManager::update(unsigned long maxMillis)
{
	while (!mQueue.empty())
	{
		IEventDataPtr pEvent = mQueue.front();
		mQueue.pop_front();

		const EventType& eventType = pEvent->VGetEventType();

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
}