#pragma once

#include "ActorComponent.h"

struct RenderComponentDesc
{
	char* MeshResourceName;
	char* TextureResourceName; 
	char* VertexShaderResourceName;
	char* PixelShaderResourceName;
};

class BaseRenderComponent : public ActorComponent
{
public:


};