#pragma once
#include <string>
#include <windows.h>

class Button
{
    float x;
    float y;
    float height;
    float width;
    std::string text;
    void (*action)();
    Button(float x,float y,float height,float width,std::string text,void (*action)());
public:
    float vert[8];
    float getX() const{return x;};
    float getY() const{return y;};
    float getWidth() const{return width;};
    float getHeight() const{return height;};
    std::string getText() const{return text;}
    void operator()(){action();};
    void render();
    class ButtonLayoutFactory
    {
        float screenW;
        float screenH;
        bool isHover=false;
        bool isDown=false;
        float freeWidth = screenW;
        float freeHeight = screenH;
        float currentX = 0;
        float currentY = 0;
        float startX = 0;
        float startY = 0;
        float pading = 25;
        public:
            Button createButton(float height,float width,std::string text,void (*action)());
            void update(float screenW, float screenH);
            ButtonLayoutFactory(float screenW, float screenH){update(screenW, screenH);};
            ButtonLayoutFactory(float screenW, float screenH,float pading):pading{pading}{update(screenW, screenH);};
            ButtonLayoutFactory(float screenW, float screenH,float startX,float startY,float pading):pading{pading},startX{startX},startY{startY},currentX{startX},currentY{startY}{update(screenW, screenH);};
    };
};



