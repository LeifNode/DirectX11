#pragma once

#include "d3dStd.h"
#include <unordered_map>
#include <DirectXMath.h>
#include "Geometry.h"

class D3DRenderer;
class RootNode;
class Actor;
class CameraNode;

typedef std::unordered_map<ActorId, ActorPtr> ActorHashTable;

class Scene
{
	friend class Actor;
public:
	Scene(D3DRenderer*);
	~Scene();

	void update(float ms);
	void render();
	WeakActorPtr getActorNode(const ActorId&) const;
	bool addChild(ActorPtr);
	bool removeChild(const ActorId&);

	bool addActorReference(ActorPtr);
	bool removeActorReference(ActorPtr);

	void setCamera(CameraNode*);
	CameraNode* getCamera(); // change me back to const

	D3DRenderer* getRenderer();

	const XMMATRIX* getTopMatrix();
	void pushAndSetMatrix(const XMMATRIX&);
	void popMatrix();

	static Scene* getScene() { return mpScene; }

private:
	static Scene* mpScene;

	RootNode* mpRoot;
	CameraNode* mpCamera;
	D3DRenderer* mpRenderer;

	MatrixStack mMatrixStack;

	ActorHashTable mActorTable;
};