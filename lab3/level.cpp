#include "level.h"
#include "GL/gl.h"
#include <math.h>


void Level::addEntity(Entity* entity)
{
    entitys.push_back(entity);
}

void Level::glInit(RECT rct)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, rct.right, rct.bottom);
    glOrtho(0, rct.right, 0, rct.bottom, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Level::reSize(float screenW, float screenH)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, screenW, screenH);
    glOrtho(0, screenW, screenH, 0, 0, 1);
}

void Level::update(float elapsedGameTime)
{
    for(Entity* entity : entitys)
    {
        if(!entity->inAir && entity->getY() > 10)
        {
            entity->inAir = true;
        }
        else if(entity->inAir && entity->getY() <= 10)
        {
            entity->inAir = false;
        }
        entity->update(elapsedGameTime);
    }
}

void Level::renderAll()
{
    for(Entity* entity : entitys)
    {
        entity->render();
    }
}
