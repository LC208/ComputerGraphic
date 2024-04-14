#pragma once
#include <string>

class Texture
{
public:
    std::string filename;
    unsigned int textureID;
    Texture(std::string filename):filename{filename}{bindTexture();};
    int width = 0;
    int height = 0;
    void bindTexture();
};
