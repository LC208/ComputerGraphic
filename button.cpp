#include "button.h"
#include <gl/gl.h>
#include <string>
#include <iostream>

Button::Button(float x,float y,float height,float width,std::string text,void (*action)()): x{x},y{y},height{height},width{width}, text{text},action{action}
{
    vert[0]=vert[6]=x;
    vert[2]=vert[4]=x+width;
    vert[1]=vert[3]=y;
    vert[5]=vert[7]=y+height;
}


void Button::render()
{
    glPushMatrix();
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0.2,1,0.2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glColor3f(0.5,0.5,0.5);
    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

Button Button::ButtonLayoutFactory::createButton(float height,float width,std::string text,void (*action)())
{
    Button btn = Button(currentX,currentY,height,width,text,action);
    if((freeWidth >= width + pading))
    {
        freeWidth -= width + pading;
        currentX += width + pading;
    }
    else if(freeHeight >= height + pading)
    {
        freeHeight -= height + pading;
        currentY += height + pading;
        freeWidth = screenW;
        currentX = startX;
        btn = Button(currentX,currentY,height,width,text,action);
    }
    return btn;
}

void Button::ButtonLayoutFactory::update(float screenW, float screenH)
{
    this->freeWidth = screenW;
    this->freeHeight = screenH;
    this->screenW = screenW;
    this->screenH = screenH;
}



