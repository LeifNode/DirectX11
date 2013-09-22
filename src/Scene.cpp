#include "Scene.h"
#include "SceneNode.h"
#include "Geometry.h"
#include "Actor.h"
#include "CameraNode.h"

Scene* Scene::mpScene;

Scene::Scene(D3DRenderer* renderer)
	:mpRenderer(renderer)
{
	mpRoot = new RootNode();
	mpScene = this;
	mpCamera = new CameraNode();
}

Scene::~Scene()
{
	delete mpCamera;
	delete mpRoot;
}

void Scene::update(float ms)
{
	mpRoot->update(ms);
}

void Scene::render()
{
	if (mpRoot && mpCamera)
	{
		mpRoot->renderChildren();
	}
}

WeakActorPtr Scene::getActorNode(const ActorId& actorId) const
{
	ActorHashTable::const_iterator actorNode = mActorTable.find(actorId);

	if (actorNode != mActorTable.end())
		return actorNode->second;

	return WeakActorPtr();
}

bool Scene::addChild(ActorPtr child)
{
	if (child->getId() != INVALID_ACTOR_ID)
	{
		mActorTable[child->getId()] = ActorPtr(child);
	}

	return mpRoot->addChild(ActorPtr(child));
}

bool Scene::removeChild(const ActorId& id)
{
	if (id == INVALID_ACTOR_ID)
		return false;

	WeakActorPtr wkActorPtr = getActorNode(id);

	//Make sure that the node was found
	if (wkActorPtr.expired())
		return false;
	
	ActorPtr actor = wkActorPtr.lock();

	if (!actor->getParent().expired())
	{
		actor->getParent().lock()->removeChild(id);
	}
	else
	{
		return mpRoot->removeChild(id);
	}

	return true;
}

bool Scene::addActorReference(ActorPtr actorRef)
{
	//std::pair<std::map<ComponentId, ActorComponentPtr>::iterator, bool> success = mActorTable.insert(std::make_pair(actorRef->getId(), WeakActorPtr(actorRef)));
	mActorTable[actorRef->getId()] = actorRef;

	return true;
}

bool Scene::removeActorReference(ActorPtr actorRef)
{
	auto i = mActorTable.find(actorRef->getId());

	if (i != mActorTable.end())
	{
		mActorTable.erase(i);
		return true;
	}

	return false;
}

void Scene::setCamera(CameraNode* camPtr)
{
	mpCamera = camPtr;
}

CameraNode* Scene::getCamera()
{
	return mpCamera;
}

D3DRenderer* Scene::getRenderer()
{
	return mpRenderer;
}

const XMMATRIX* Scene::getTopMatrix()
{
	return mMatrixStack.getTop();
}

void Scene::pushAndSetMatrix(const XMMATRIX& toWorld)
{
	mMatrixStack.pushAndSet(toWorld);
}

void Scene::popMatrix()
{
	mMatrixStack.pop();
}