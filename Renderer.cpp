
#include "Renderer.hpp"


Renderer::Renderer(float2 size)
{
    int i=0;
    _xAngle = _yAngle = 0.0;
    _cube = (float4*)malloc(sizeof(float4) * 24);

    // ----- Front
    _cube[i++] = (float4){ 1.0, 1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, 1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, -1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ 1.0, -1.0, 1.0, 1.0 };
    
    // ----- Back
    _cube[i++] = (float4){ 1.0, 1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, 1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, -1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ 1.0, -1.0, -1.0, 1.0 };

    // ----- Top
    _cube[i++] = (float4){ 1.0, 1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, 1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, 1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ 1.0, 1.0, -1.0, 1.0 };

    // ----- Bottom
    _cube[i++] = (float4){ 1.0, -1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, -1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, -1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ 1.0, -1.0, -1.0, 1.0 };

    // ----- Left
    _cube[i++] = (float4){ -1.0, 1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, 1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, -1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ -1.0, -1.0, 1.0, 1.0 };

    // ----- Right
    _cube[i++] = (float4){ 1.0, 1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ 1.0, -1.0, 1.0, 1.0 };
    _cube[i++] = (float4){ 1.0, -1.0, -1.0, 1.0 };
    _cube[i++] = (float4){ 1.0, 1.0, -1.0, 1.0 };

    _vertexCount = i;

    _frame.left = 0;
    _frame.right = size.x;
    _frame.top = 0;
    _frame.bottom =size.y;

}

Renderer::~Renderer()
{
    free(_cube);
}

void Renderer::drawInContext(HDC context)
{
    //drawCube(context);
    drawRandom(context);
}

void Renderer::drawRandom(HDC context)
{
    HDC hDCMem = CreateCompatibleDC(context);
    HDC hDCMem2 = CreateCompatibleDC(context);
    DWORD* data, *data2;
    BITMAPINFO info;
    ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = _frame.right;
	info.bmiHeader.biHeight = _frame.bottom;  //negative so (0,0) is at top left
    info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;

    HBITMAP hbm = CreateDIBSection(hDCMem, &info, DIB_RGB_COLORS, (VOID**) &data, NULL, 0);
    HBITMAP hbm2 = CreateDIBSection(hDCMem2, &info, DIB_RGB_COLORS, (VOID**) &data2, NULL, 0);

    SelectObject(hDCMem, hbm);
    SelectObject(hDCMem2, hbm2);

    COLORREF greyColor = 2101101010;
    COLORREF lastColor = 0;

    for(int y=0; y < _frame.bottom; y++)
    {
        for(int x=0; x< _frame.right; x++)
        {   
            //GetHardware32(&greyColor);
            
            greyColor = rand() & (rand() << 16);
            //greyColor |= (greyColor << 8);
            //greyColor |= (greyColor << 16);
            
            data[y * _frame.right + x] = greyColor;
            //data[y * _frame.right + x] = (greyColor + lastColor) << 2;
        }
    }
    /*
    float coeff[5] = { 0.053990967, 0.241970725, 0.39894228, 0.241970725, 0.053990967 };
    for(int y=0; y < _frame.bottom; y++)
    {
        for(int x=0; x< _frame.right; x++)
        {   
            float sum = 0;
            for(int i=0; i<5; i++)
            {
                if(x + i >=0 && x + i < _frame.right)
                    sum += (coeff[i] * (float)data[y * _frame.right + x]);
            }
            data2[y * _frame.right + x] = (int)(sum / 5.0);
        }
    }
    */

    BitBlt(context, 0, 0, _frame.right, _frame.bottom, hDCMem, 0, 0, SRCCOPY);
}

void Renderer::drawCube(HDC context)
{
    std::vector<float4> vertexBuffer;

    _yAngle += 0.01;

    if(_yAngle > 2 * 3.1459)
        _yAngle -= 2 * 3.1459;

    matrix_F4x4 scale, perspective, rotate;
    scaleF(0.5, 0.5, 0.5, &scale);
    rotateF(_xAngle, _yAngle, 0.0, &rotate);
    perspectiveF(1.0, 3.14 / 4.0, 1.0, 100.0, &perspective);

    Gdiplus::Graphics graphics(context);
    Gdiplus::Pen      pen(Gdiplus::Color(0, 0, 255, 255));
    Gdiplus::Color blackColor(0, 0, 0, 255);
    Gdiplus::Color greyColor(128, 128, 128, 255);
    graphics.Clear(blackColor);
    for(int i=0; i<_vertexCount; i++)
    {
        float4 temp, out;
        matrix_multiply_f4(&rotate, &_cube[i], &out);
        matrix_multiply_f4(&scale, &out, &temp);
        temp.z -= 10.0;
        matrix_multiply_f4(&perspective, &temp, &out);
        out.x /= out.w;
        out.y /= out.w;
        out.z /= out.w;
        
        vertexBuffer.push_back(out);
    }

    HDC Memhdc;
	HDC hdc;
	HBITMAP Membitmap;
    Memhdc = CreateCompatibleDC(hdc);
	Membitmap = CreateCompatibleBitmap(hdc, _frame.right, _frame.bottom);

	SelectObject(Memhdc, Membitmap);
    
    
    for(int i=0; i<_vertexCount / 8; i++)
    {
        graphics.DrawLine(&pen, vertexBuffer[i * 4].x * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4].y * _frame.bottom + _frame.bottom / 2, 
                                vertexBuffer[i * 4 + 1].x  * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4 + 1].y  * _frame.bottom + _frame.bottom / 2 );
        graphics.DrawLine(&pen, vertexBuffer[i * 4 + 1].x  * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4 + 1].y  * _frame.bottom + _frame.bottom / 2, 
                                vertexBuffer[i * 4 + 2].x * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4 + 2].y  * _frame.bottom + _frame.bottom / 2 );
        graphics.DrawLine(&pen, vertexBuffer[i * 4 + 2].x * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4 + 2].y  * _frame.bottom + _frame.bottom / 2, 
                                vertexBuffer[i * 4 + 3].x * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4 + 3].y  * _frame.bottom + _frame.bottom / 2 );
        graphics.DrawLine(&pen, vertexBuffer[i * 4 + 3].x * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4 + 3].y  * _frame.bottom + _frame.bottom / 2, 
                                vertexBuffer[i * 4].x * _frame.right + _frame.right / 2, 
                                vertexBuffer[i * 4].y * _frame.bottom + _frame.bottom / 2);
    }

    BitBlt(hdc, 0, 0, _frame.right, _frame.bottom, Memhdc, 0, 0, SRCCOPY);
	DeleteObject(Membitmap);
	DeleteDC    (Memhdc);
	DeleteDC    (hdc);
}

RECT* Renderer::frame()
{
    return &_frame;
}

void Renderer::incYAngle()
{
    _yAngle += 0.025;
    if(_yAngle > 3.1459 * 2)
        _yAngle -= 3.1459 * 2;
}

void Renderer::decYAngle()
{
    _yAngle -= 0.025;
    if(_yAngle < 0)
        _yAngle += 3.1459 * 2;
}

void Renderer::incXAngle()
{
    _xAngle += 0.025;
    if(_xAngle > 3.1459 * 2)
        _xAngle -= 3.1459 * 2;
}

void Renderer::decXAngle()
{
    _xAngle -= 0.025;
    if(_xAngle < 0)
        _xAngle += 3.1459 * 2;
}

void Renderer::GetHardware32(unsigned long *number)
{
    unsigned char passed;
        
    asm volatile ("rdrand %0\t\n"
                  "setc %1\t\n"
                  : "=r" (*number), "=qm" (passed));
    if(!passed)
    {
        // !!! Critical: There was an unknow error reading the RDRAND instruction !!!
        *number = 0;
    }
}