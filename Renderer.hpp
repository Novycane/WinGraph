#include <windows.h>
#include <Wingdi.h>
#include <gdiplus.h>
#include <vector>
#include <iostream>

#include "C:\Users\snovak\Documents\Software Projects\C\Graphics\Math\Polygon.h"
#include "C:\Users\snovak\Documents\Software Projects\C\Graphics\Math\Affine.h"


class Renderer
{
    public:
        Renderer(float2 size);
        ~Renderer();
        void drawInContext(HDC context);

        void incYAngle();
        void decYAngle();

        void incXAngle();
        void decXAngle();

        RECT* frame();

    private:
        unsigned int _vertexCount;
        float _yAngle;
        float _xAngle;
        RECT _frame;
        float4* _cube;

        void drawCube(HDC context);
        void drawRandom(HDC context);
        void GetHardware32(unsigned long *number);


}; // end class