#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <DXErr.h>

#include "vertexbufferdx11.h"
#include "pixelshaderdx11.h"
#include "vertexshaderdx11.h"

class DX11
{
public:
	DX11(int width, int height, HWND hwnd, bool fullscreen);
	~DX11(void);
	ID3D11Device* Device();
	ID3D11DeviceContext* DeviceContext();
	void Clear(D3DXCOLOR bgColor);
	void SwapBuffers();
	void SetSize(int width, int height);
	VertexBufferDX11* CreateVertexBuffer(int vertexSize, int numberOfVertices, void* data);
	VertexShaderDX11* CreateVertexShader(LPCWSTR srcFile, LPCSTR functionName);
	PixelShaderDX11* CreatePixelShader(LPCWSTR srcFile, LPCSTR functionName);

private:
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
};

