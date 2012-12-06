#include "pixelshaderdx11.h"
#include "dx11.h"

PixelShaderDX11::PixelShaderDX11(LPCWSTR srcFile, LPCSTR functionName, DX11* dx)
{
	mDX = dx;
	ID3D10Blob* file;

	D3DX11CompileFromFile(srcFile, 0, 0, functionName, "ps_5_0", 0, 0, 0, &file, 0, 0);
	mDX->Device()->CreatePixelShader(file->GetBufferPointer(), file->GetBufferSize(), NULL, &mPixelShader);
}


PixelShaderDX11::~PixelShaderDX11(void)
{
	mPixelShader->Release();
}

void PixelShaderDX11::Set()
{
	mDX->DeviceContext()->PSSetShader(mPixelShader, 0, 0);
}