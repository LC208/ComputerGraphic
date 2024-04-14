#include "menu.h"
#include "windows.h"
#include "element.h"
#include <iostream>
#include <GL/gl.h>

void Menu::addElement(Element* element)
{
    if(Menu::elements.size() > 0)
    {
        switch(layout)
        {
        case XRL :
            {
                Element *prev = Menu::elements[Menu::elements.size()-1];
                if(element->getX() + element->getWidth() <= screenW)
                {
                    addStartX(element->getWidth() + pading);
                    Menu::elements.push_back(element);
                }
                else if (element->getY()  + element->getHeight() <= screenH)
                {
                    addStartY(element->getHeight() + pading);
                    Menu::elements.push_back(element);
                }
            }
            return;
        case YUD :
            {
                Element* prev = Menu::elements[Menu::elements.size()-1];
                if(element->getY() +  element->getHeight() <= screenH)
                {
                    addStartY(element->getHeight() + pading);
                    Menu::elements.push_back(element);
                }
                else if (element->getX()  +  element->getWidth() <= screenW)
                {
                    addStartX(element->getWidth() + pading);
                    Menu::elements.push_back(element);
                }
            }
            return;
        }
    }
    else
    {
        if(element->getX() + element->getWidth() <= screenW && element->getY() + element->getHeight() <= screenH)
        {
            Menu::elements.push_back(element);
            layout == XRL ? addStartX(element->getWidth()+ pading) : addStartY(element->getHeight()+ pading);
        }
    }

}


void Menu::createButton(float width,float height,std::string text,void (*action)())
{
    addElement(new Button(startX,startY,width,height,text,action));
}

void Menu::createText(std::string text,float width,float height,void (*action)() ,ElementType type)
{
    addElement(new Text(text,startX,startY,width,height,action,type));
}

void Menu::createImage(Texture texture,float width, float height, void (*action)(), ElementType type)
{
    addElement(new Image(texture,width,height,startX,startY,action,type));
}


void Menu::reSize(float screenW, float screenH)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, screenW, screenH);
    glOrtho(0, screenW, screenH, 0, 0, 1);
    for(Element* element: this->elements)
    {
        element->update(screenW / this->screenW,screenH / this->screenH );
    }
    this->screenH = screenH;
    this->screenW = screenW;
}

void Menu::handleClick(HWND hwnd)
{
    POINT cursorXY;
    if(GetCursorPos(&cursorXY) && ScreenToClient(hwnd,&cursorXY))
    {
        for(Element* btn : elements)
        {
            if(btn->getType() == ElementType::Active && cursorXY.x >= btn->getX() && cursorXY.x<= btn->getX() + btn->getWidth() && cursorXY.y >= btn->getY() && cursorXY.y <= btn->getY() + btn->getHeight())
            {
                (*btn)();
            }
        }
    }
}

void Menu::glInit(RECT rct)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, rct.right, rct.bottom);
    glOrtho(0, rct.right, rct.bottom, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Menu::renderAll()
{
    for(Element* element : elements)
    {
        element->render();
    }
}

void Menu::addStartX(float x)
{
    this->startX += x;
}

void Menu::addStartY(float y)
{
    this->startY += y;
}
