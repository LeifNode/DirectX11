#include "Actor.h"
#include "Scene.h"
#include "ActorComponent.h"

Actor::Actor(ActorId id)
	:mId(id)
{
}

Actor::~Actor()
{
}

void Actor::setTransform(const XMMATRIX *toWorld, const XMMATRIX *fromWorld)
{

}

void Actor::getTransform(XMMATRIX *toWorldOut, XMMATRIX *fromWorldOut)
{

}

HRESULT Actor::update(float const elapsedMs)
{
	for (auto i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		i->second->VUpdate(elapsedMs);
	}

	for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		(*i)->update(elapsedMs);
	}

	return S_OK;
}

HRESULT Actor::onRestore()
{
	return S_OK;
}

HRESULT Actor::preRender()
{
	for (auto i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		i->second->VPreRender();
	}

	return S_OK;
}

bool Actor::isVisible() const
{
	return true;
}

HRESULT Actor::render()
{
	for (auto i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		i->second->VRender();
	}

	return S_OK;
}

HRESULT Actor::renderChildren()
{
	for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		if ((*i)->preRender() == S_OK)
		{
			(*i)->render();
			(*i)->renderChildren();
			(*i)->postRender();
		}
	}

	return S_OK;
}

HRESULT Actor::postRender()
{
	for (auto i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		i->second->VPostRender();
	}

	return S_OK;
}

bool Actor::addComponent(ActorComponentPtr pComponent)
{
	std::pair<std::map<ComponentId, ActorComponentPtr>::iterator, bool> success = mComponents.insert(std::make_pair(pComponent->VGetId(), pComponent));

	//mComponents[pComponent->VGetId()] = pComponent;

	/*if (success.second)
	{
		if (pComponent->VGetName() == "TransformComponent")
		{
			mTransformComponent = pComponent;
		}
	}*/

	pComponent->mpOwner = this;

	return success.second;
}

bool Actor::addChild(ActorPtr child)
{
	if (!child->getParent().expired())
		child->getParent().lock()->removeChild(child->getId());

	mChildren.push_back(child);
	child->mpParent = WeakActorPtr(Scene::getScene()->getActorNode(mId));
	Scene::getScene()->addActorReference(child);

	return true;
}

bool Actor::removeChild(ActorId id)
{
	for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		if ((*i)->getId() == id)
		{
			Scene::getScene()->removeActorReference(*i);
			mChildren.erase(i);
			return true;
		}
	}

	return false;
}