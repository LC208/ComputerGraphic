#include <GL/gl.h>
#include <iostream>
#include "menu.h"

Menu::Image::Image(Texture texture,float width, float height, float x, float y, void (*action)(), ElementType type) : Element{x,y,width,height,action,type}, texture{texture}
{
    vert[0]=vert[6]=x;
    vert[2]=vert[4]=x+width;
    vert[1]=vert[3]=y;
    vert[5]=vert[7]=y+height;
    this->texture.bindTexture();
}

void Menu::Image::render()
{
    vert[0]=vert[6]=getX();
    vert[2]=vert[4]=getX() + getWidth();
    vert[1]=vert[3]=getY();
    vert[5]=vert[7]=getY() + getHeight();
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->texture.textureID);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //cord[1] = cord[3] = 0;
    //cord[5] = cord[7] = 1;
    //cord[2] = cord[4] = 1;
    //cord[0] = cord[6] = 0;
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glTexCoordPointer(2, GL_FLOAT, 0, cord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}

