#include <vector>
#include "C:\Users\snovak\Documents\Software Projects\C\Graphics\Math\Polygon.h"

#ifndef FRAGMENT_HPP
#define FRAGMENT_HPP

typedef struct
{
    int x;
    int y;
    int length;
} Span;

class Fragment
{
public:

    void CreateFragment(TriangleF4* face, int2* frame);

private:
    std::vector<Span> _spans;

}; // End Fragment Class

#endif