#include "vertexshaderdx11.h"
#include "dx11.h"

VertexShaderDX11::VertexShaderDX11(LPCWSTR srcFile, LPCSTR functionName, DX11* dx)
{
	HRESULT hr;

	mDX = dx;
	mLayout = NULL;
	hr = D3DX11CompileFromFile(srcFile, 0, 0, functionName, "vs_5_0", 0, 0, 0, &mFile, 0, 0);
 	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not load shader."), MB_OK);
	}	
	mDX->Device()->CreateVertexShader(mFile->GetBufferPointer(), mFile->GetBufferSize(), NULL, &mVertexShader);
}


VertexShaderDX11::~VertexShaderDX11(void)
{
	mVertexShader->Release();
	mLayout->Release();
	mFile->Release();
}

void VertexShaderDX11::Set()
{
	//Set input layout between vertex buffer and vertex shader
	if (mLayout == NULL)
		mDX->Device()->CreateInputLayout(&mInputElementDescription[0], mInputElementDescription.size(), mFile->GetBufferPointer(), mFile->GetBufferSize(), &mLayout);
	mDX->DeviceContext()->IASetInputLayout(mLayout);

	//Set vertex shader
	mDX->DeviceContext()->VSSetShader(mVertexShader, 0, 0);

}

void VertexShaderDX11::AddElementDescription(LPCSTR SemanticName, UINT SemanticIndex,DXGI_FORMAT Format, 
	UINT InputSlot, UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC ied;
	ied.SemanticName = SemanticName;
	ied.SemanticIndex = SemanticIndex;
	ied.Format = Format;
	ied.InputSlot = InputSlot;
	ied.AlignedByteOffset = AlignedByteOffset;
	ied.InputSlotClass = InputSlotClass;
	ied.InstanceDataStepRate = InstanceDataStepRate;

	mInputElementDescription.push_back(ied);
}