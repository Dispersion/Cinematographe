#include "vertexbufferdx11.h"
#include "dx11.h"
#include <exception>


VertexBufferDX11::VertexBufferDX11(int vertexSize, int numberOfVertices, void* data, DX11* dx)
{
	HRESULT hr;

	mDX = dx; 
	mVertexSize = vertexSize;
	mNumberOfVertices = numberOfVertices;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;               
	bd.ByteWidth = vertexSize*numberOfVertices;   
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;   

	hr = mDX->Device()->CreateBuffer(&bd, NULL, &pVBuffer); 
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not create vertex buffer."), MB_OK);
	}	
	
	D3D11_MAPPED_SUBRESOURCE ms;
	hr =  mDX->DeviceContext()->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); 
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not map vertex buffer."), MB_OK);	
	}	

	memcpy(ms.pData, data, numberOfVertices*vertexSize);
	mDX->DeviceContext()->Unmap(pVBuffer, NULL);
}

VertexBufferDX11::~VertexBufferDX11(void)
{
	pVBuffer->Release();
}

void VertexBufferDX11::Draw()
{        
	//Select which vertex buffer to display.
    UINT stride = mVertexSize;
    UINT offset = 0;
    mDX->DeviceContext()->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    //Select which primtive type we are using.
    mDX->DeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Draw the vertex buffer to the back buffer.
    mDX->DeviceContext()->Draw(mNumberOfVertices, 0);
}