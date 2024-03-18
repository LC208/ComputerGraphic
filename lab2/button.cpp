#include "menu.h"
#include "element.h"
#include <gl/gl.h>
#include <string>

Menu::Button::Button(float x,float y,float width,float height,std::string text,void (*action)()): Element{x,y,width,height,action,ElementType::Active}, text{text}
{
    vert[0]=vert[6]=x;
    vert[2]=vert[4]=x+width;
    vert[1]=vert[3]=y;
    vert[5]=vert[7]=y+height;
}


void Menu::Button::render()
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

    this->t.render();
    glPopMatrix();
}

void Menu::Button::update()
{
    vert[0]=vert[6]=getX();
    vert[2]=vert[4]=getX()+getWidth();
    vert[1]=vert[3]=getY();
    vert[5]=vert[7]=getY()+getHeight();
    t.setX(getX());
    t.setY(getY());
    t.update();
}



