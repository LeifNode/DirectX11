#pragma once

#include "ActorComponent.h"
#include "Geometry.h"

class BoxRenderComponent : public ActorComponent
{
public:
	BoxRenderComponent(ActorPtr pOwner, ID3D11VertexShader* vertShaderPtr, ID3D11PixelShader* pixelShaderPtr);
	virtual ~BoxRenderComponent();

	virtual void VRender();

	virtual const char* VGetName() const { return gName; }
	
public:
	static const char* gName;

private:
	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxIB;
	ID3D11Buffer* mTransformBuffer;

	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
};