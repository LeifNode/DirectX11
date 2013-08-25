#pragma once

#include "d3dStd.h"
#include <vector>
#include <map>
#include <memory>
#include <Windows.h>
#include <DirectXMath.h>
#include <string>

class Actor;
class BaseRenderComponent;

using DirectX::XMMATRIX;

enum AlphaType
{
	AlphaOpaque,
	AlphaTexture,
	AlphaMaterial,
	AlphaVertex
};

class PassNode;

class RootNode
{
public:
	RootNode();
	~RootNode();

	void update(float dt);

	void renderChildren();

	bool addChild(ActorPtr child, RenderPass pass = RenderPass_Actor);
	bool removeChild(ActorId id);

private:
	std::vector<PassNode*> mPasses;
};

class PassNode
{
public:
	PassNode();
	~PassNode();

	void update(float dt);

	void renderChildren();

	bool addChild(ActorPtr child);
	bool removeChild(ActorId id);

private:
	std::map<ActorId, ActorPtr> mActorNodes;
};

//class SceneNodeProperties
//{
//	friend class SceneNode;
//
//protected:
//	ActorId                 mActorId;
//	std::string				mName;
//	XMMATRIX				mToWorld, mFromWorld;
//	RenderPass				mRenderPass;
//	Material				mMaterial;
//	AlphaType				mAlphaType;
//
//	void SetAlpha(const float alpha) 
//		{ mAlphaType=AlphaMaterial; mMaterial.SetAlpha(alpha); } 
//
//public:
//    SceneNodeProperties(void);
//	const ActorId &ActorId() const { return mActorId; }
//	XMMATRIX const &ToWorld() const { return mToWorld; }
//	XMMATRIX const &FromWorld() const { return mFromWorld; }
//	void getTransforms(XMMATRIX *toWorldOut, XMMATRIX *fromWorldOut) const;
//
//	const char * Name() const { return mName.c_str(); }		
//
//	bool HasAlpha() const { return mMaterial.HasAlpha(); }						
//	float Alpha() const { return mMaterial.GetAlpha(); }
//	AlphaType AlphaType() const { return mAlphaType; }
//
//	RenderPass RenderPass() const { return mRenderPass; }
//
//	Material GetMaterial() const { return mMaterial; }
//};

//class SceneNode : public ISceneNode
//{
//	friend class Scene;
//
//public:
//	SceneNode(const std::string& type, RenderPass renderPass, const XMMATRIX *toWorld, const XMMATRIX *fromWorld=NULL);
//	virtual ~SceneNode() { };
//
//	virtual const SceneNodeProperties * const VGetProperties() const { return &mProps; }
//
//	virtual void VSetTransform(const XMMATRIX *toWorld, const XMMATRIX *fromWorld=NULL);
//	virtual void VGetTransform(XMMATRIX *toWorldOut, XMMATRIX *fromWorldOut);
//
//	virtual HRESULT VOnUpdate(Scene *pScene, float const elapsedMs);
//	virtual HRESULT VOnRestore(Scene *pScene);
//
//	virtual HRESULT VPreRender(Scene *pScene);
//	virtual bool VIsVisible(Scene *pScene) const;
//	virtual HRESULT VRender(Scene *pScene) { return S_OK; }
//	virtual HRESULT VRenderChildren(Scene *pScene);
//	virtual HRESULT VPostRender(Scene *pScene);
//
//	virtual bool VAddChild(SceneNodePtr child);
//	virtual bool VRemoveChild(ActorId id);
//	virtual HRESULT VOnLostDevice(Scene *pScene);
//
//protected:
//	SceneNodeList mChildren;
//	SceneNode *mpParent;
//	SceneNodeProperties mProps;
//
//	BaseRenderComponent* mpRenderComponent;
//};