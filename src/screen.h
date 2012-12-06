#pragma once
#include "dx11\dx11.h"
#include "canvas.h"

class Screen : public LumiereRenderer::Canvas
{
public:
	Screen(int width, int height, DX11* dx);
	~Screen(void);
	void Draw();
	void Done();

private:
	DX11* mDX;
	VertexBufferDX11* mVertices;
	ID3D11Texture2D* mTexture;
	ID3D11SamplerState* mTextureSamplerState;	
	ID3D11ShaderResourceView* mTextureResource;

	VertexShaderDX11* vs;
	PixelShaderDX11* ps;
	int mWidth, mHeight;	
};

