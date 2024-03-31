#include "menu.h"
#include "element.h"
#include <string>
#include "stb-master/stb_easy_font.h"
#include <GL/gl.h>

Menu::Text::Text(std::string text, float x, float y,float width, float height,void (*action)(), ElementType type): text{text},
Element{x,y,width,height,action,type}
{
    this->num_quads = stb_easy_font_print(0,0,const_cast<char*>(text.data()),0,this->buffer,sizeof(this->buffer));
}

void Menu::Text::render()
{
    this->num_quads = stb_easy_font_print(0,0,const_cast<char*>(getText().data()),0,this->buffer,sizeof(this->buffer));
    glPushMatrix();
    glColor3f(0.5,0.5,0.5);
    glTranslatef(getX()+ (getWidth()-stb_easy_font_width(const_cast<char*>(text.data()))*(getWidth()/stb_easy_font_width(const_cast<char*>(text.data()))))/2.0,getY()+ (getHeight()-stb_easy_font_height(const_cast<char*>(text.data()))*(getHeight()/stb_easy_font_height(const_cast<char*>(text.data()))))/2.0 + 2*(getHeight()/stb_easy_font_height(const_cast<char*>(text.data()))),0);
    glScalef(getWidth()/stb_easy_font_width(const_cast<char*>(text.data())), getHeight()/stb_easy_font_height(const_cast<char*>(text.data())), 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, this->buffer);
    glDrawArrays(GL_QUADS, 0, this->num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

