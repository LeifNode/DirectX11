#pragma once

//Implmentation adapted from GCC4
//Stripped multithreading code and multiple Event queue support since they were not needed

#include <strstream>
#include <ostream>
#include <istream>
#include <memory>
#include <list>
#include <map>
#include "Templates.h"
#include "..\3rdParty\FastDelegate\FastDelegate.h"

class IEventData;

typedef unsigned long EventType;
typedef std::shared_ptr<IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;

#define REGISTER_EVENT(eventClass) EventSystem::get()Register<eventClass>(eventClass::sk_EventType)
#define CREATE_EVENT(eventType) gEventFactory.Create(eventType)

class IEventData
{
public:
	virtual ~IEventData() {}
	virtual const EventType& getEventType() const = 0;
	virtual float getTimeStamp() const = 0;
	//virtual void Serialize(std::ostrstream& out) const = 0;
    //virtual void Deserialize(std::istrstream& in) = 0;
	virtual IEventDataPtr copy() const = 0;
    virtual const char* getName() const = 0;
};

class BaseEventData : public IEventData
{
    const float m_timeStamp;

public:
	explicit BaseEventData(const float timeStamp = 0.0f) : m_timeStamp(timeStamp) { }

	// Returns the type of the event
	virtual const EventType& getEventType() const = 0;

	virtual float getTimeStamp() const { return m_timeStamp; }

	// Serializing for network input / output
	//virtual void Serialize(std::ostrstream &out) const	{ }
    //virtual void Deserialize(std::istrstream& in) { }
};

class EventSystem
{
    typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<IEventDataPtr> EventQueue;

	EventListenerMap mEventListeners;
    EventQueue mQueue;

	GenericObjectFactory<IEventData, EventType> mEventFactory;

	static EventSystem* mpInstance;

public:
	EventSystem();
	~EventSystem();

	static EventSystem* get();

    bool addListener(const EventType& type, const EventListenerDelegate& eventDelegate);
    bool removeListener(const EventType& type, const EventListenerDelegate& eventDelegate);

    bool triggerEvent(const IEventDataPtr& pEvent) const;
    bool queueEvent(const IEventDataPtr& pEvent);
    //bool abortEvent(const EventType& type, bool allOfType = false);

	void clear();

    bool update(unsigned long maxMillis = 0xffffffff);

	//Will probably go unused, but I will leave them in in case I want to serialize events and recreate them
	template <class T>
	bool registerEvent(EventType eventId)
	{
		mEventFactory.Register<T>(eventId);
	}

	IEventDataPtr create(EventType eventId)
	{
		mEventFactory.Create(eventId);
	}
};
