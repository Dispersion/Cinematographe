#pragma once
#include <d3dx11.h>

class DX11;

class ShaderDX11
{
public:
	ShaderDX11();
	~ShaderDX11(void);
	virtual void Set() = 0;

protected:
	DX11* mDX;
};

