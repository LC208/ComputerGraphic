#pragma once
#include "renderctl.h"
#include "entity.h"
#include <vector>
#include "collisionmanager.h"
#include <string>

class Level : public RenderCtl
{
public:
    void renderAll() override;
    void update(float elapsedGameTime) override;
    void reSize(float screenW, float screenH) override;
    void glInit(RECT rct) override;
    void addEntity(Entity* entity, bool);
    void loadLevel(const std::string&);
    void exportLevel(const std::string& filename);
    void KeyHandling();
    float gravity = 9.8;
    MoveabelEntity * hero;
    void renderBackground();

private:
    std::vector<Entity*> entitys;
};

