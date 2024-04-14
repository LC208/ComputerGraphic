#include "tileset.h"
#include <iostream>

float * TileSet::getVertOfTile(int tile, int line, EnttitySide side)
{
    float *cord = new float[8];
    cord[1] = cord[3] = startPos.y/(texture.height) + (tileHeight*line)/(texture.height) + (tileHeight/(texture.height));
    cord[5] = cord[7] = startPos.y/(texture.height) + (tileHeight*line)/(texture.height);
    cord[2] = cord[4] = startPos.x/(texture.width) + (tileWidth*tile)/(texture.width) + (side == EnttitySide::RIGHT ? 1 : 0)*(tileWidth/(texture.width));
    cord[0] = cord[6] = startPos.x/(texture.width) + (tileWidth*tile)/(texture.width) + (side == EnttitySide::LEFT ? 1 : 0)*(tileWidth/(texture.width));
    return cord;
}


