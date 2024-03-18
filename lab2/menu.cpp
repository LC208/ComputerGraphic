#include "menu.h"
#include "windows.h"
#include "element.h"
#include <iostream>

void Menu::addElement(Element* element)
{
    if(Menu::elements.size() > 0)
    {
        switch(layout)
        {
        case XRL :
            {
                Element *prev = Menu::elements[Menu::elements.size()-1];
                if(startX + element->getWidth() <= screenW)
                {
                    addStartX(element->getWidth() + pading);
                    Menu::elements.push_back(element);
                }
                else if (startY  + element->getHeight() <= screenH)
                {
                    addStartY(element->getHeight() + pading);
                    Menu::elements.push_back(element);
                }
            }
            return;
        case YUD :
            {
                Element* prev = Menu::elements[Menu::elements.size()-1];
                if(startY +  element->getHeight() <= screenH)
                {
                    addStartY(element->getHeight() + pading);
                    Menu::elements.push_back(element);
                }
                else if (startX  +  element->getWidth() <= screenW)
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
        if(startX + element->getWidth() <= screenW && startY + element->getHeight() <= screenH)
        {
            Menu::elements.push_back(element);
            layout == XRL ? addStartX(element->getWidth()+ pading) : addStartY(element->getHeight()+ pading);
        }
    }

}

void Menu::createButton(float width,float height,std::string text,void (*action)())
{
    addElement(new Button(startX, startY,width,height,text,action));
}

void Menu::createText(std::string text,float width,float height,void (*action)() ,ElementType type)
{
    addElement(new Text(text,startX, startY,width,height,action,type));
}

void Menu::createImage(std::string filename,float width, float height,unsigned int textureID, void (*action)(), ElementType type)
{
    addElement(new Image(filename,width,height,textureID,startX, startY,action,type));
}


void Menu::update(float screenW, float screenH)
{
    this->screenW = screenW;
    this->screenH = screenH;
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
