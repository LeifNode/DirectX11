#include "SceneNode.h"
#include "Scene.h"
#include "Actor.h"

RootNode::RootNode()
{
	for (unsigned i = 0; i <= RenderPass_Last; i++)
		mPasses.push_back(new PassNode());
}

RootNode::~RootNode()
{
	for (auto i = mPasses.begin(); i != mPasses.end(); ++i)
	{
		delete *i;
	}

	mPasses.clear();
}

void RootNode::update(float dt)
{
	for (auto i = mPasses.begin(); i != mPasses.end(); ++i)
	{
		(*i)->update(dt);
	}
}

void RootNode::renderChildren()
{
	for (auto i = mPasses.begin(); i != mPasses.end(); ++i)
	{
		(*i)->renderChildren();
	}
}

bool RootNode::addChild(ActorPtr child, RenderPass pass)
{
	Scene::getScene()->addActorReference(child);
	return mPasses[pass]->addChild(child);
}

bool RootNode::removeChild(ActorId id)
{
	for (auto i = mPasses.begin(); i != mPasses.end(); ++i)
	{
		if ((*i)->removeChild(id))
			return true;
	}

	return false;
}


//Pass node
PassNode::PassNode()
{}

PassNode::~PassNode()
{}

void PassNode::update(float dt)
{
	for (auto i = mActorNodes.begin(); i != mActorNodes.end(); ++i)
	{
		i->second->update(dt);
	}
}

void PassNode::renderChildren()
{
	for (auto i = mActorNodes.begin(); i != mActorNodes.end(); ++i)
	{
		if (i->second->preRender() == S_OK)
		{
			i->second->render();
			i->second->renderChildren();
			i->second->postRender();
		}
	}
}

bool PassNode::addChild(ActorPtr child)
{
	auto i = mActorNodes.find(child->getId());

	if (i != mActorNodes.end())
		return false;

	if (!child->getParent().expired())
		child->mpParent = WeakActorPtr();

	mActorNodes[child->getId()] = child;
	return true;
}

bool PassNode::removeChild(ActorId id)
{
	auto i = mActorNodes.find(id);

	if (i != mActorNodes.end())
	{
		Scene::getScene()->removeActorReference(i->second);
		mActorNodes.erase(i);
		return true;
	}

	return false;
}

//SceneNodeProperties::SceneNodeProperties()
//	:
//	mActorId(INVALID_ACTOR_ID),
//	mRenderPass(RenderPass_0),
//	mAlphaType(AlphaOpaque)
//{}
//
//void SceneNodeProperties::getTransforms(XMMATRIX *toWorldOut, XMMATRIX *fromWorldOut) const
//{
//	if (toWorldOut)
//		*toWorldOut = mToWorld;
//	
//	if (fromWorldOut)
//		*fromWorldOut = mFromWorld;
//}
//
//
//SceneNode::SceneNode(std::string name, RenderPass renderPass, const XMMATRIX *toWorld, const XMMATRIX *fromWorld)
//{
//	mpParent = NULL;
//	mProps.mActorId = actorId;
//	mProps.mName = (renderComponent) ? renderComponent->VGetName() : "SceneNode";
//	mProps.mRenderPass = renderPass;
//	mProps.mAlphaType = AlphaOpaque;
//	VSetTransform(toWorld, fromWorld);
//}
//
//void SceneNode::VSetTransform(const XMMATRIX *toWorld, const XMMATRIX *fromWorld)
//{
//	mProps.mToWorld = *toWorld;
//
//	if (!fromWorld)
//	{
//		mProps.mFromWorld = DirectX::XMMatrixInverse(NULL, mProps.mToWorld);
//	}
//	else
//	{
//		mProps.mFromWorld = *fromWorld;	
//	}
//}
//
//
//void SceneNode::VGetTransform(XMMATRIX *toWorldOut, XMMATRIX *fromWorldOut)
//{
//	VGetProperties()->getTransforms(toWorldOut, fromWorldOut);
//}
//
//HRESULT SceneNode::VOnRestore(Scene *pScene)
//{
//	SceneNodeList::iterator i = mChildren.begin();
//	SceneNodeList::iterator end = mChildren.end();
//
//	while (i != end)
//	{
//		(*i)->VOnRestore(pScene);
//	}
//	return S_OK;
//}
//
//HRESULT SceneNode::VOnUpdate(Scene *pScene, float const elapsedMs)
//{
//	SceneNodeList::iterator i = mChildren.begin();
//	SceneNodeList::iterator end = mChildren.end();
//
//	while (i != end)
//	{
//		(*i)->VOnUpdate(pScene, elapsedMs);
//	}
//	return S_OK;
//}
//
//HRESULT SceneNode::VPreRender(Scene *pScene)
//{
//	pScene->pushAndSetMatrix(mProps.ToWorld());
//	return S_OK;
//}
//
//bool SceneNode::VIsVisible(Scene *pScene) const
//{
//	return true;
//}
//
//HRESULT SceneNode::VRenderChildren(Scene *pScene)
//{
//	for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
//	{
//		if((*i)->VPreRender(pScene) == S_OK)
//		{
//			(*i)->VRender(pScene);
//			(*i)->VRenderChildren(pScene);
//			(*i)->VPostRender(pScene);
//		}
//	}
//	return S_OK;
//}
//
//HRESULT SceneNode::VPostRender(Scene *pScene)
//{
//	pScene->popMatrix();
//	return S_OK;
//}
//
//bool SceneNode::VAddChild(SceneNodePtr child)
//{
//	//omitted bounding sphere in favor of later implementation of bounding box
//
//	mChildren.push_back(child);
//	
//	reinterpret_cast<SceneNode*>(child.get())->mpParent = this;
//
//	return true;
//}
//
//bool SceneNode::VRemoveChild(ActorId id)
//{
//	for (SceneNodeList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
//	{
//		const SceneNodeProperties* nodeProperties = (*i)->VGetProperties();
//		if (nodeProperties->ActorId() != INVALID_ACTOR_ID && nodeProperties->ActorId() == id)
//		{
//			mChildren.erase(i);
//			return true;
//		}
//	}
//
//	return false;
//}
//
//HRESULT SceneNode::VOnLostDevice(Scene *pScene)
//{
//	for (auto i = mChildren.begin(); i != mChildren.end(); ++i)
//	{
//		(*i)->VOnLostDevice(pScene);
//	}
//}