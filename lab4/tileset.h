#pragma once
#include "texture.h"
#include "const.h"
#include <map>
#include "vec2.h"

class TileSet
{
public:
    Texture texture;
    float tileWidth;
    vec2f startPos;
    float tileHeight;
    float* getVertOfTile(int tile, int line, EnttitySide side);
    TileSet(Texture texture, float tileWidth, float tileHeight, vec2f startPos): texture{texture}, tileWidth{tileWidth}, tileHeight{tileHeight},startPos{startPos}{};
};

