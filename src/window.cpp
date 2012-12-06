#include "Window.h"
#include <map>
#include <iostream>
#include "msghandler.h"
#include <list>

using namespace std;

static Window* gWindow;

static LRESULT CALLBACK MsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (gWindow)
	{
		if ( uMsg == WM_DESTROY )
		{
			PostQuitMessage(0);
			return 0;
		}
		return gWindow->WndProc(uMsg, wParam, lParam);
	}
	return DefWindowProc (hwnd, uMsg, wParam, lParam);	
}

Window::Window(unsigned int w, unsigned int h, bool fullscreen)
{
	mWidth = w;
	mHeight = h;
	mFullscreen = fullscreen;	

	HINSTANCE hInstance = GetModuleHandle(NULL);

	//Setup window
	WNDCLASSEX winclass;
	winclass.cbSize        = sizeof(WNDCLASSEX);
	winclass.style         = CS_DBLCLKS;
	winclass.lpfnWndProc   = &MsgHandler;
	winclass.cbClsExtra    = 0;
	winclass.cbWndExtra    = 0;
	winclass.hInstance     = hInstance;
	winclass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
	winclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
	winclass.hbrBackground = GetSysColorBrush(COLOR_APPWORKSPACE);
	winclass.lpszMenuName  = MAKEINTRESOURCE(3); 
	winclass.lpszClassName = L"program";
	winclass.hIconSm       = NULL;

	RegisterClassEx(&winclass);

	DWORD style;

	if (mFullscreen)
	{
		style = WS_POPUP;
	}
	else
	{
		style =  WS_OVERLAPPEDWINDOW;
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, L"program", L"Cinematographe", style, 0, 0,	mWidth,	mHeight, NULL,
		(HMENU)0, hInstance, NULL);

	UpdateWindow(hwnd);

	gWindow = this;

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	RECT windowRect;
	GetClientRect(hwnd, &windowRect);

	mWidth = windowRect.right - windowRect.left;
	mHeight = windowRect.bottom - windowRect.top;

	mLeftMouseButtonDown = false;
	mRightMouseButtonDown = false;
	mMiddleMouseButtonDown = false;

	mLeftDrag = false;
	mIdleFunc = NULL;
	mResizeFunc = NULL;
	mDX11 = NULL;
}

Window::~Window(void)
{
	delete(mDX11);
}

LRESULT CALLBACK Window::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{

	case WM_SIZE:
		{
			mDX11->SetSize(LOWORD(lParam), HIWORD(lParam));

			if (mResizeFunc)
			{
				mResizeFunc(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND Window::GetHandle()
{
	return hwnd;
}

void Window::SetIdleFunc(void (*idle)())
{
	mIdleFunc = idle;
}

void Window::SetResizeFunc(void (*resize)(int w, int h))
{
	mResizeFunc = resize;
}


void Window::MainLoop()
{
	MSG  msg;
	while(true)
	{
		if ( PeekMessage(&msg,NULL,0,0,0) )
		{
			if ( !GetMessage(&msg,NULL,0,0) ) break;				
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else
		{				
			if (mIdleFunc)
				mIdleFunc();
		}
	}
}

DX11* Window::RenderContext()
{
	if (mDX11 == NULL)	
		mDX11 = new DX11(mWidth, mHeight, hwnd, mFullscreen);

	return mDX11;
}

unsigned int Window::Width()
{
	return mWidth;
}

unsigned int Window::Height()
{
	return mHeight;
}
