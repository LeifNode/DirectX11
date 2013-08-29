#pragma once

//Implmentation adapted from GCC4

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

extern GenericObjectFactory<IEventData, EventType> gEventFactory;
#define REGISTER_EVENT(eventClass) gEventFactory.Register<eventClass>(eventClass::sk_EventType)
#define CREATE_EVENT(eventType) gEventFactory.Create(eventType)

class IEventData 
{
public:
	virtual ~IEventData(void) {}
	virtual const EventType& VGetEventType(void) const = 0;
	virtual float GetTimeStamp(void) const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
    virtual void VDeserialize(std::istrstream& in) = 0;
	virtual IEventDataPtr VCopy(void) const = 0;
    virtual const char* GetName(void) const = 0;
};

class BaseEventData : public IEventData
{
    const float m_timeStamp;

public:
	explicit BaseEventData(const float timeStamp = 0.0f) : m_timeStamp(timeStamp) { }

	// Returns the type of the event
	virtual const EventType& VGetEventType(void) const = 0;

	float GetTimeStamp(void) const { return m_timeStamp; }

	// Serializing for network input / output
	virtual void VSerialize(std::ostrstream &out) const	{ }
    virtual void VDeserialize(std::istrstream& in) { }
};

class EventManager
{
    typedef std::list<EventListenerDelegate> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<IEventDataPtr> EventQueue;

public:
	EventManager();
	~EventManager();

	static EventManager* get();

    bool addListener(const EventType& type, const EventListenerDelegate& eventDelegate);
    bool removeListener(const EventType& type, const EventListenerDelegate& eventDelegate);

    bool triggerEvent(const IEventDataPtr& pEvent) const;
    bool queueEvent(const IEventDataPtr& pEvent);
    //bool abortEvent(const EventType& type, bool allOfType = false);

    bool update(unsigned long maxMillis = 0xffffffff);

private:
	EventListenerMap mEventListeners;
    EventQueue mQueue;

	static EventManager* mpInstance;
};
