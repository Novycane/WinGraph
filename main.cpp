#include<windows.h>
#include<gdiplus.h>
#include<objidl.h>
#include<iostream>

#include"Renderer.hpp"

Renderer* _renderer;
const char g_szClassName[] = "Test!";

void OnPaint(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Pen      pen(Gdiplus::Color(255, 0, 0, 255));
    graphics.DrawLine(&pen, 0, 0, 200, 100);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
	switch (msg)
	{
        case WM_KEYDOWN:
            if(wParam == VK_LEFT)
                _renderer->incYAngle();
            else if(wParam == VK_RIGHT)
                _renderer->decYAngle();
            else if(wParam == VK_UP)
                _renderer->decXAngle();
            else if(wParam == VK_DOWN)
                _renderer->decXAngle();
            else
                printf("Other\n");
            InvalidateRect(hwnd,_renderer->frame() , FALSE);
            RedrawWindow(hwnd , NULL , NULL , RDW_INVALIDATE);
            break;
        case WM_TIMER:
            InvalidateRect(hwnd,_renderer->frame() , FALSE);
            RedrawWindow(hwnd , NULL , NULL , RDW_INVALIDATE);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            //OnPaint(hdc);
            _renderer->drawInContext(hdc);
            EndPaint(hwnd, &ps);
      return 0;
        case WM_MBUTTONUP:
            std::cout << msg << std::endl;
            break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "What an App!",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    SetWindowPos(hwnd, 0, 0, 0, 500, 500, SWP_SHOWWINDOW);

    _renderer = new Renderer((float2) {500.0, 500.0});

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    SetTimer(hwnd, 1, 34, NULL);
    
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return Msg.wParam;
}