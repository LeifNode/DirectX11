#pragma once

#include "ActorComponent.h"
#include "Geometry.h"

class SphereRenderComponent : public ActorComponent
{
public:
	SphereRenderComponent(ID3D11VertexShader* pVertShader, ID3D11PixelShader* pPixelShader, float radius = 1.0f, int sliceCount = 10, int stackCount = 10);
	virtual ~SphereRenderComponent();

	virtual void VRender();

	virtual const char* VGetName() const { return gName; }

public:
	static const char* gName;

private:
	ID3D11Buffer* mSphereVB;
	ID3D11Buffer* mSphereIB;
	ID3D11Buffer* mTransformBuffer;

	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;

	unsigned mIndexCount;
};