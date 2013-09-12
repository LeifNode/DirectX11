#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Interfaces.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <cassert>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <list>
#include <string>
#include <algorithm>
#include "dxerr.h"

using namespace DirectX;

using std::shared_ptr;
using std::weak_ptr;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;

//Testing speed impact of disabling SIMD on the DirectXMath library
//#ifndef _XM_NO_INTRINSICS_
//#define _XM_NO_INTRINSICS_
//#endif

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(const_cast<WCHAR*>((WCHAR*)__FILE__), (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 

namespace Colors
{
	const XMFLOAT4 White = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	const XMFLOAT4 Black = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	const XMFLOAT4 Red   = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	const XMFLOAT4 Green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	const XMFLOAT4 Blue  = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
}

/*void* operator new(size_t memSize) _THROW1(_STD bad_alloc)
{
	void* p;
	static const size_t alignment = 16;

	p = _aligned_malloc(memSize, alignment);

	if (p == NULL)
	{
		static const std::bad_alloc nomem;
		_RAISE(nomem);
	}

	return p;
}

void* operator new[](size_t memSize) _THROW1(_STD bad_alloc)
{
	void* p;
	static const size_t alignment = 16;

	p = _aligned_malloc(memSize, alignment);

	if (p == NULL)
	{
		static const std::bad_alloc nomem;
		_RAISE(nomem);
	}

	return p;
}*/