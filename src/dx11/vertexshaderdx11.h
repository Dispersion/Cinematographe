#pragma once
#include <vector>
#include <d3dx11.h>

class DX11;

class VertexShaderDX11
{
public:
	VertexShaderDX11(LPCWSTR srcFile, LPCSTR functionName, DX11* dx);
	~VertexShaderDX11(void);
	void Set();
	void AddElementDescription(LPCSTR SemanticName, UINT SemanticIndex,DXGI_FORMAT Format, 
		UINT InputSlot, UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, 
		UINT InstanceDataStepRate);
private:
	DX11* mDX;
	ID3D11VertexShader* mVertexShader;
	ID3D11InputLayout* mLayout;
	ID3D10Blob* mFile;
	std::vector<D3D11_INPUT_ELEMENT_DESC> mInputElementDescription;
};

