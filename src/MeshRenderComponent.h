#pragma once

#include "ActorComponent.h"
#include "Geometry.h"

class MeshRenderComponent : public ActorComponent
{
public:
	MeshRenderComponent(const Mesh& mesh, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader);
	virtual ~MeshRenderComponent();

	virtual void VRender();

	virtual const char* VGetName() const { return gName; }

public:
	static const char* gName;

private:
	ID3D11Buffer* mMeshVB;
	ID3D11Buffer* mMeshIB;
	ID3D11Buffer* mTransformBuffer;

	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;

	unsigned mIndexCount;
};