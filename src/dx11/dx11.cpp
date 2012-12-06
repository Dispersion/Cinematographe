#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment (lib, "DXErr.lib")

#include "dx11.h"

DX11::DX11(int width, int height, HWND hwnd, bool fullscreen)
{
	HRESULT hr;

	//Buffer description
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//SwapChain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc; 
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd; 
	swapChainDesc.Windowed = !fullscreen; 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, NULL, &mDeviceContext);
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not create device.."), MB_OK);
	}	

	ID3D11Texture2D* BackBuffer;
	hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&BackBuffer );
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not get buffer."), MB_OK);
	}	

	//Create Render Target
	hr = mDevice->CreateRenderTargetView( BackBuffer, NULL, &mRenderTargetView );
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not create render target view."), MB_OK);
	}	
	BackBuffer->Release();

	//Set Render Target
	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, NULL );
}


DX11::~DX11(void)
{
	mSwapChain->SetFullscreenState(FALSE, NULL);
	mSwapChain->Release();
	mDevice->Release();
	mDeviceContext->Release();
}

ID3D11Device* DX11::Device()
{
	return mDevice;
}

ID3D11DeviceContext* DX11::DeviceContext()
{
	return mDeviceContext;
}

void DX11::Clear(D3DXCOLOR bgColor)
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bgColor);
}

void DX11::SwapBuffers()
{
	mSwapChain->Present(0, 0);
}

void DX11::SetSize(int width, int height)
{
	if (mSwapChain)
	{
		mDeviceContext->OMSetRenderTargets(0, 0, 0);	
		mRenderTargetView->Release();

		HRESULT hr;
		hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if(FAILED(hr))
		{	
			MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not resize buffers."), MB_OK);
		}	

		ID3D11Texture2D* pBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D),
			(void**) &pBuffer );

		if(FAILED(hr))
		{	
			MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not get buffers."), MB_OK);
		}	

		hr = mDevice->CreateRenderTargetView(pBuffer, NULL,
			&mRenderTargetView);

		if(FAILED(hr))
		{	
			MessageBox(NULL, DXGetErrorDescription(hr),	TEXT("Could not create render target view."), MB_OK);
		}	

		pBuffer->Release();
		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, NULL );
	}
}

VertexBufferDX11* DX11::CreateVertexBuffer(int vertexSize, int numberOfVertices, void* data)
{
	return new VertexBufferDX11(vertexSize, numberOfVertices, data, this);
}

VertexShaderDX11* DX11::CreateVertexShader(LPCWSTR srcFile, LPCSTR functionName)
{
	return new VertexShaderDX11(srcFile, functionName, this);
}

PixelShaderDX11* DX11::CreatePixelShader(LPCWSTR srcFile, LPCSTR functionName)
{
	return new PixelShaderDX11(srcFile, functionName, this);
}