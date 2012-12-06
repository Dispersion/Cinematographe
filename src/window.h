#pragma once
#include <windows.h>
#include "dx11\dx11.h"

class Window
{
public:
	Window( unsigned int w = 640, unsigned int h = 480, bool fullscreen = false);
	~Window(void);

	LRESULT CALLBACK WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	HWND GetHandle();
	void ReleaseMouse( );
	void SetIdleFunc(void (*idle)());
	void SetResizeFunc(void (*resize)(int w, int h));
	void MainLoop();
	DX11* RenderContext();
	unsigned int Width();
	unsigned int Height();

protected:
	HWND hwnd;
	HDC	 hdc;
	HGLRC hrc;

	bool mFullscreen;
	unsigned int mWidth;
	unsigned int mHeight;

	bool mLeftMouseButtonDown;
	bool mRightMouseButtonDown;
	bool mMiddleMouseButtonDown;

	bool mLeftDrag;	
	void (*mIdleFunc)();
	void (*mResizeFunc)(int w, int h);
	DX11* mDX11;
};
