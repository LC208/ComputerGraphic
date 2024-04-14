#pragma once
#include "renderctl.h"
#include "entity.h"
#include <vector>
#include "collisionmanager.h"

class Level : public RenderCtl
{
public:
    void renderAll() override;
    void update(float elapsedGameTime) override;
    void reSize(float screenW, float screenH) override;
    void glInit(RECT rct) override;
    void addEntity(Entity* entity);
    void loadLevel(const std::string&);
    void exportLevel(const std::string& filename);
    void KeyHandling();
    float gravity = 9.8;
    MoveabelEntity * hero;
    //Texture levelTexture{std::string("Tiles.png")};
   /* std::string levelMap[10] =
    {
        "BBBBBBBBBB",
        "BNNNNNNNNB",
        "BNNNNNNNNB",
        "BNNNNNNNNB",
        "BNNNNNNNNB",
        "BNNNNNNNNB",
        "BNNNNNNNNB",
        "BNNNNNNNNB",
        "BSNNNNNNNB",
        "BBBBBBBBBB",
    };*/

private:
    std::vector<Entity*> entitys;
};

