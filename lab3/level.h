#pragma once
#include "renderctl.h"
#include "entity.h"
#include <vector>


class Level : public RenderCtl
{
public:
    void renderAll() override;
    void update(float elapsedGameTime) override;
    void reSize(float screenW, float screenH) override;
    void glInit(RECT rct) override;
    void addEntity(Entity* entity);
    float gravity = 9.8;
private:
    std::vector<Entity*> entitys;
};

