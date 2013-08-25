#pragma once

#include <vector>
#include <memory>
#include <Windows.h>
#include <DirectXMath.h>

class Actor;
class ActorComponent;

typedef std::shared_ptr<Actor> ActorPtr;
typedef std::weak_ptr<Actor> WeakActorPtr;

typedef std::shared_ptr<ActorComponent> ActorComponentPtr;
typedef std::weak_ptr<ActorComponent> WkActorComponentPtr;

typedef unsigned ActorId;
typedef unsigned ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

class Scene;
//class SceneNodeProperties;

//using DirectX::XMMATRIX;

//class ISceneNode
//{
//public:
//	virtual const SceneNodeProperties * const VGetProperties() const=0;
//
//	virtual void VSetTransform(const XMMATRIX *toWorld, const XMMATRIX *fromWorld)=0;
//	virtual void VGetTransform(XMMATRIX *toWorldOut, XMMATRIX *fromWorldOut)=0;
//
//	virtual HRESULT VOnUpdate(Scene *pScene, float const elapsedMs)=0;
//	virtual HRESULT VOnRestore(Scene *pScene)=0;
//
//	virtual HRESULT VPreRender(Scene *pScene)=0;
//	virtual bool VIsVisible(Scene *pScene) const=0;
//	virtual HRESULT VRender(Scene *pScene)=0;
//	virtual HRESULT VRenderChildren(Scene *pScene)=0;
//	virtual HRESULT VPostRender(Scene *pScene)=0;
//
//	virtual bool VAddChild(std::shared_ptr<ISceneNode> child)=0;
//	virtual bool VRemoveChild(ActorId id)=0;
//	virtual HRESULT VOnLostDevice(Scene *pScene)=0;
//
//	virtual ~ISceneNode() { };
//};
//
//typedef std::shared_ptr<ISceneNode> SceneNodePtr;
//typedef std::vector<SceneNodePtr> SceneNodeList;

enum RenderPass
{
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_NotRendered,
	RenderPass_Last
};

class IRenderableComponent
{
	virtual HRESULT VPreRender(Scene*)=0;
	virtual HRESULT VRender(Scene*)=0;
	virtual HRESULT VPostRender(Scene*)=0;

	virtual ~IRenderableComponent() { };
};

