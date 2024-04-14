#pragma once
#include <string>
#include <vector>
#include <windef.h>
#include "stb-master/stb_easy_font.h"
#include <algorithm>
#include "element.h"
#include "renderctl.h"
#include "texture.h"

class Menu : public RenderCtl
{
    public:
        enum Layout{XRL, YUD};
        void createButton(float width,float heigth,std::string text,void (*action)());
        void createText(std::string text,float width,float height,void (*action)() ,ElementType type);
        void createImage(Texture texture,float width, float height, void (*action)(), ElementType type);
        void reSize(float screenW, float screenH) override;
        void glInit(RECT rct) override;
        void update(float) override{};
        void handleClick(HWND hwnd);
        void renderAll() override;
        void addStartX(float x);
        void addStartY(float y);
        void changeLayout(Layout l){this->layout = l;};
        Menu(float screenW, float screenH){reSize(screenW, screenH);};
        Menu(float screenW, float screenH,float pading):pading{pading}{reSize(screenW, screenH);};
        Menu(float screenW, float screenH,float startX,float startY,float pading):pading{pading},startX{startX},startY{startY}{reSize(screenW, screenH);};
    private:
        float screenW;
        float screenH;
        void addElement(Element* element);
        class Text : public Element
        {
            std::string text;
            int num_quads;
            float buffer[50*20]; // 20 - textLength
        public:
            std::string getText() const{return text;}
            Text(std::string text, float x, float y, float width, float height, void (*action)(), ElementType type);
            void render() override;
        };
        class Button : public Element
        {
            std::string text;
            float vert[8];
            Text t{text,getX(),getY(),getWidth(),getHeight(), nullptr, ElementType::Nonactive};
        public:
            std::string getText() const{return text;}
            void render() override;
            void update(float screenW, float screenH) override;
            bool isHover=false;
            bool isDown=false;
            bool isHidden = false;
            Button(float x,float y,float width,float height,std::string text,void (*action)());
        };
        class Image: public Element
        {
            Texture texture;
            float vert[8];
            float cord[8] = {0,0, 1,0, 1,1, 0,1};
        public:
            Image(Texture texture,float width, float height, float x, float y, void (*action)(), ElementType type);
            void render() override;
        };
        Layout layout{YUD};
        float startX = 0;
        float startY = 0;
        float pading = 25;
        std::vector<Element*> elements;
};
