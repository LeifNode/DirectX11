#pragma once

#include "d3dStd.h"

class TiXmlElement;
class ActorComponent;
class TransformComponent;
class BaseRenderComponent;

typedef std::string ActorType;

class Actor
{
	friend class PassNode;

public:
	Actor(ActorId id);
	~Actor();

	void initialize(TiXmlElement*);

	void setTransform(const XMMATRIX *toWorld, const XMMATRIX *fromWorld);
	void getTransform(XMMATRIX *toWorldOut, XMMATRIX *fromWorldOut);

	HRESULT update(float const elapsedMs);
	HRESULT onRestore();

	HRESULT preRender();
	bool isVisible() const;
	HRESULT render();
	HRESULT renderChildren();
	HRESULT postRender();

	bool addChild(ActorPtr child);
	bool removeChild(ActorId id);

	WeakActorPtr getParent() const { return mpParent; }
	ActorId getId() const { return mId; }

	bool addComponent(ActorComponentPtr pComponent);

	template <class T>
	weak_ptr<T> getComponent(ComponentId id)
	{
		auto i = mComponents.find(id);
		if (i != mComponents.end())
		{
			ActorComponentPtr base(i->second);
			shared_ptr<T> sub(static_pointer_cast<T>(base));
			return weak_ptr<T>(sub);
		}
		else
			return weak_ptr<T>();
	}

	template <class T>
    weak_ptr<T> getComponent(const char *name)
    {
		ComponentId id = ActorComponent::GetIdFromName(name);
        std::map<ComponentId, ActorComponentPtr>::iterator i = mComponents.find(id);
        if (i != mComponents.end())
        {
            ActorComponentPtr pBase(i->second);
            shared_ptr<T> pSub(static_pointer_cast<T>(pBase));
            return weak_ptr<T>(pSub);
        }
        else
        {
            return weak_ptr<T>();
        }
    }

	//weak_ptr<TransformComponent> Transform();
	//weak_ptr<BaseRenderComponent> Renderer();


private:
	ActorId mId;

	WeakActorPtr mpParent;
	std::vector<ActorPtr> mChildren;

	std::map<ComponentId, ActorComponentPtr> mComponents;
	//weak_ptr<TransformComponent> mTransformComponent;
};