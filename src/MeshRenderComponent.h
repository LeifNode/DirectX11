#pragma once

#include "ActorComponent.h"
#include "Geometry.h"

class MeshRenderComponent : public ActorComponent
{
	MeshRenderComponent(Mesh& mesh, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader);
	virtual ~MeshRenderComponent();
};