#include "menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include <GL/gl.h>
#include <iostream>

Menu::Image::Image(std::string filename,float width, float height,unsigned int textureID, float x, float y, void (*action)(), ElementType type) :  Element{x,y,width,height,action,type},textureID{textureID},filename{filename}
{
    int tcnt = 4;
    int sW;
    int sH;
    unsigned char *data=stbi_load(const_cast<char*>(filename.data()),&sW,&sH,&tcnt,0);
    vert[0]=vert[6]=x;
    vert[2]=vert[4]=x+width;
    vert[1]=vert[3]=y;
    vert[5]=vert[7]=y+height;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sW, sH,
    0, tcnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Menu::Image::render()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
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

void Menu::Image::update()
{
    vert[0]=vert[6]=getX();
    vert[2]=vert[4]=getX()+getWidth();
    vert[1]=vert[3]=getY();
    vert[5]=vert[7]=getY()+getHeight();
}
