#pragma once

#include <strstream>
#include <ostream>
#include <istream>
#include <memory>
#include "3rdParty\FastDelegate\FastDelegate.h"

class IEventData;

typedef unsigned long EventType;
typedef std::shared_ptr<IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;

