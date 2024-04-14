#pragma once
#include <windef.h>
#include <iostream>
class RenderCtl
{
public:
    virtual void renderAll() = 0;
    virtual void update(float elapsedGameTime){std::cout << elapsedGameTime;};
    virtual void reSize(float screenW, float screenH) = 0;
    virtual void glInit(RECT rct) = 0;
};
