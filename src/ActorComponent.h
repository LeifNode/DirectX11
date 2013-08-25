#pragma once

#include "d3dStd.h"
#include "String.h"
#include "Actor.h"

class ActorComponent
{
	friend class Actor;

public:
	ActorComponent() : mpOwner(NULL) { }
	virtual ~ActorComponent() { mpOwner = NULL; }

	virtual void VInit(TiXmlElement*) { }

	virtual void VUpdate(float dt) { }
	
	virtual void VPreRender() { }
	virtual void VRender() { }
	virtual void VPostRender() { }

	virtual ComponentId VGetId(void) const { return GetIdFromName(VGetName()); }
	virtual const char *VGetName() const=0;
	static ComponentId GetIdFromName(const char* componentStr)
	{
		void* rawId = HashedString::hash_name(componentStr);
		return reinterpret_cast<ComponentId>(rawId);
	}
protected:
	Actor* mpOwner;
};