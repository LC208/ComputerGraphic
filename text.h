#pragma once
#include <string>
class Text
{
    float x;
    float y;
    std::string text;
    float buffer[50*text.length()];
    int num_quads;
    float width;
    float height
public:
    Text(std::string text, float x, float y, float width, float height);
    void render();
};
