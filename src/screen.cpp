#include "screen.h"

struct VERTEX{FLOAT X, Y, Z; D3DXVECTOR2 texture;}; 

Screen::Screen(int width, int height, DX11* dx)
{
	HRESULT hr;
	mDX = dx;

	mWidth = width;
	mHeight = height;
	mFormat = LumiereRenderer::SENSOR_FORMAT_R32G32B32A32;

	mData = new float[width*height*4];
	ZeroMemory(&((float*)mData)[0], width*height*4*sizeof(float));

	//Init vertex buffer.
	VERTEX OurVertices[] =
	{
		{-width*0.5f,  height*0.5f, 10.0f, D3DXVECTOR2(0.0f, 1.0f)},
		{ width*0.5f, -height*0.5f, 10.0f, D3DXVECTOR2(1.0f, 0.0f)},
		{-width*0.5f, -height*0.5f, 10.0f, D3DXVECTOR2(0.0f, 0.0f)},
		{-width*0.5f,  height*0.5f, 10.0f, D3DXVECTOR2(0.0f, 1.0f)},
		{ width*0.5f,  height*0.5f, 10.0f, D3DXVECTOR2(1.0f, 1.0f)},
		{ width*0.5f, -height*0.5f, 10.0f, D3DXVECTOR2(1.0f, 0.0f)}
	};

	mVertices = dx->CreateVertexBuffer(sizeof(VERTEX), 6, OurVertices);

	//Init texture.
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.SampleDesc.Quality = 0;

	mTexture = NULL;
	hr = dx->Device()->CreateTexture2D(&desc, NULL, &mTexture);
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not create texture2D."), MB_OK);
	}	

	//Create texture resource.
	D3D11_SHADER_RESOURCE_VIEW_DESC TextureOutputDescription = { desc.Format, D3D11_SRV_DIMENSION_TEXTURE2D, 0, 0 };
	TextureOutputDescription.Texture2D.MipLevels = 1;
	dx->Device()->CreateShaderResourceView( mTexture, &TextureOutputDescription, &mTextureResource );


	//Create texture sampler.
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = dx->Device()->CreateSamplerState( &sampDesc, &mTextureSamplerState );

	//Init shader.
	vs = dx->CreateVertexShader(L"shaders/shader.hlsl", "VShader");
	vs->AddElementDescription("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	vs->AddElementDescription("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ps = dx->CreatePixelShader(L"shaders/shader.hlsl", "PShader");
}


Screen::~Screen(void)
{
}

void Screen::Draw()
{
	//Set shaders.
	vs->Set();
	ps->Set();

	//Upload texture to pixel shader.
	mDX->DeviceContext()->PSSetShaderResources( 0, 1, &mTextureResource );
	mDX->DeviceContext()->PSSetSamplers( 0, 1, &mTextureSamplerState );

	//Draw vertex buffer.
	mVertices->Draw();
}

void Screen::Done()
{
	//When the rendering is done, we upload the texture to the graphics card.
	D3D11_MAPPED_SUBRESOURCE data;
	mDX->DeviceContext()->Map(mTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	float* pTexels = (float*)data.pData;
	for( INT row = 0; row < mHeight; row++ )
	{
		//Row picth gives the result as number of bytes, since a float is 4 bytes, we have 
		//to divide by 4 to get the number of floats in a row.
		UINT rowStart = row * data.RowPitch/4;
		for( INT col = 0; col < mWidth; col++ )
		{
			UINT colStart = col * 4;
			pTexels[rowStart + colStart + 0] = ((float*)mData)[row * (mWidth*4) + (col*4) + 0];
			pTexels[rowStart + colStart + 1] = ((float*)mData)[row * (mWidth*4) + (col*4) + 1];
			pTexels[rowStart + colStart + 2] = ((float*)mData)[row * (mWidth*4) + (col*4) + 2];
			pTexels[rowStart + colStart + 3] = ((float*)mData)[row * (mWidth*4) + (col*4) + 3];
		}
	}

	mDX->DeviceContext()->Unmap(mTexture, 0);
}