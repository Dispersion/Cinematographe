#pragma once
#include <d3dx11.h>

class DX11;

class PixelShaderDX11
{
public:
	PixelShaderDX11(LPCWSTR srcFile, LPCSTR functionName, DX11* dx);
	~PixelShaderDX11(void);
	void Set();

private:
	DX11* mDX;
	ID3D11PixelShader* mPixelShader;
};

