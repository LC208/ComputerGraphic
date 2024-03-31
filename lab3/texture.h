#pragma once
#include <string>

class Texture
{
public:
    std::string filename;
    unsigned int textureID;
    Texture(std::string filename, unsigned int textureID):filename{filename}, textureID{textureID}{};
    void bindTexture();
};
