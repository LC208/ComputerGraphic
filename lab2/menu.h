#pragma once
#include <string>
#include <vector>
#include "windef.h"
#include "stb-master/stb_easy_font.h"
#include <algorithm>
#include "element.h"

class Menu
{
    public:

        enum Layout{XRL, YUD};
        void createButton(float width,float heigth,std::string text,void (*action)());
        void createText(std::string text,float width,float height,void (*action)() ,ElementType type);
        void createImage(std::string filename,float width, float height,unsigned int textureID, void (*action)(), ElementType type);
        void update(float screenW, float screenH);
        void handleClick(HWND hwnd);
        void renderAll();
        void addStartX(float x);
        void addStartY(float y);
        void changeLayout(Layout l){this->layout = l;};
        Menu(float screenW, float screenH){update(screenW, screenH);};
        Menu(float screenW, float screenH,float pading):pading{pading}{update(screenW, screenH);};
        Menu(float screenW, float screenH,float startX,float startY,float pading):pading{pading},startX{startX},startY{startY}{update(screenW, screenH);};
    private:
        float screenW;
        float screenH;
        void addElement(Element* element);
        class Text : public Element
        {
            std::string text;
            int num_quads;
            float buffer[50*20];
        public:
            std::string getText() const{return text;}
            Text(std::string text, float x, float y, float width, float height, void (*action)(), ElementType type);
            void render() override;
            void update() override;
        };
        class Button : public Element
        {
            std::string text;
            float vert[8];
            Text t{text,getX(),getY(),getWidth(),getHeight(), nullptr, ElementType::Nonactive};
        public:
            std::string getText() const{return text;}
            void render() override;
            void update() override;
            bool isHover=false;
            bool isDown=false;
            bool isHidden = false;
            Button(float x,float y,float width,float height,std::string text,void (*action)());
        };
        class Image: public Element
        {
            std::string filename;
            float buffer[50*20];
            float vert[8];
            float cord[8] = {0,0, 1,0, 1,1, 0,1};
            unsigned int textureID;
        public:
            Image(std::string filename,float width, float height,unsigned int textureID, float x, float y, void (*action)(), ElementType type);
            void render() override;
            void update() override;
        };

        Layout layout{YUD};
        float startX = 0;
        float startY = 0;
        float pading = 25;
        std::vector<Element*> elements;
};
