#pragma once

#include "d3dStd.h"

class D3DApp;

class D3DRenderer
{
public:
	D3DRenderer();
	~D3DRenderer();

	bool initialize();
	void onResize();

	void buildInputLayouts(ID3DBlob*);

	inline ID3D11Device* device() const { return md3dDevice; }
	inline ID3D11DeviceContext* context() const { return md3dImmediateContext; }

	void preRender();
	void postRender();

	//HRESULT compileVertexShader(WCHAR* szFileName, LPCSTR szEntryPoint, ID3D11VertexShader* pVertexShaderOut);
	//HRESULT compilePixelShader(WCHAR* szFileName, LPCSTR szEntryPoint,  ID3D11PixelShader* pPixelShaderOut);

	ID3D11InputLayout* getInputLayout(const std::string&) const;

	static HRESULT compileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

private:
	UINT m4xMsaaQuality;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTarget;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;

	D3D_DRIVER_TYPE md3dDriverType;

	std::map<std::string, ID3D11InputLayout*> mInputLayouts;

	bool mEnable4xMsaa;

	float mClearColor[4];
};