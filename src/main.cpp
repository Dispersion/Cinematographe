#include <Windows.h>
#include <vector>
#include "window.h"
#include "dx11/dx11.h"
#include "screen.h"
#include "pinhole.h"
#include "ccd.h"


Window* window;
DX11* dx;

ID3D11Buffer* cbPerObjectBuffer;
Screen* screen;

XMMATRIX ProjectionMatrix;

using namespace LumiereRenderer;

Camera* camera;

void resize(int w, int h)
{
	//Update viewport with new size.
	D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = float(w);
    viewport.Height = float(h);

	dx->DeviceContext()->RSSetViewports(1, &viewport);
	
	//Update projection matrix with new size.
	ProjectionMatrix = XMMatrixOrthographicLH((float)w, (float)h, 1.0f, 100.0f);
}

void init()
{
	//Create a constant buffer for the shader.
	D3D11_BUFFER_DESC cbbd;	
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(XMMATRIX);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	dx->Device()->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
	
	//Resize to the client area of the window.
	resize(window->Width(), window->Height());
}

void idle()
{
	//Render image to a texture.
	for (int n = 0; n < 300*400; n++)
	{
		//Generate a sample from the image sensor.
		ImageSensorSample imageSensorSample = camera->GetImageSensor()->Sample();
		
		//Generate a ray from the image sensor sample.
		Ray ray = camera->GenerateRay(imageSensorSample);

		//Trace Scene

		//If we hit anything save the result in the image sensor.
		camera->GetImageSensor()->SetIrradiance(imageSensorSample, 1);
	}

	//Clear backbuffer with a color.
	dx->Clear(D3DXCOLOR( 0.0f, 0.2f, 0.4f, 1.0f )); 

	//Update constants in the shader
	dx->DeviceContext()->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &XMMatrixTranspose(ProjectionMatrix), 0, 0 );
	dx->DeviceContext()->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
	
	//Draw the screen.
	screen->Draw();
    
	//Present the backbuffer to the screen
	dx->SwapBuffers();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//Setup the window.
	window = new Window(800, 600);
	window->SetIdleFunc(idle);
	window->SetResizeFunc(resize);

	//Get the rendering context.
	dx = window->RenderContext();

	//Setup the canvas.
	screen = new Screen(400, 300, dx);
		
	init();

	//Setup camera.
	camera = new Pinhole(0.02f);
	camera->SetImageSensor(new CCD(0.04f, 0.03f, 400, 300)); 
	camera->GetImageSensor()->SetCanvas(screen);	
	
	window->MainLoop();
	return 0;
}





