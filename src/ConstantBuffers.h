#pragma once

#include "d3dStd.h"

struct CBPerObject
{
	XMMATRIX World;
	XMMATRIX WorldViewProj;
	XMMATRIX WorldInvTranspose;
	XMMATRIX TextureTransform;
};