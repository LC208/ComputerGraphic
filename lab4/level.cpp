#include "level.h"
#include "GL/gl.h"
#include <math.h>
#include "collisionmanager.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


void Level::addEntity(Entity* entity)
{
    CollisionManager::colTable[entity] = CollisionBox{entity->tileset.tileWidth,entity->tileset.tileHeight,entity->pos};
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


void Level::exportLevel(const std::string& filename)
{
    /*ofstream file(filename, ios::binary);
    if(!file.is_open())
    {
        cerr << "Failed to open file" << endl;
        return;
    }
    file.write(reinterpret_cast<const char*>(this), sizeof(*this));
    file.close();
    cout << "Level exported" << endl;*/
}


void Level::loadLevel(const std::string& filename)
{
    std::ifstream f(filename);
    json data = json::parse(f);

    std::map<EnttityAnimationState, std::array<int,3>> actionToLineAndTiles;
    actionToLineAndTiles[EnttityAnimationState::STAYING] = {0, 0, 1};
    actionToLineAndTiles[EnttityAnimationState::WALKING] = {0, 8, 1};
    actionToLineAndTiles[EnttityAnimationState::JUMPING] = {1, 6, 0};

    Texture charac{std::string("spritelist.png")};
    Texture testtex{std::string("jungle_pack_05.png")};
    hero = new MoveabelEntity(TileSet{charac,80,80,{0,0}},20,100,80,80,1,actionToLineAndTiles);
    addEntity(hero);

    json blocks = data["blocks"];
    for(int i = 0; i < blocks.size();i++)
    {
        std::string type = blocks[i]["name"];
        if(type == "block" || type == "wall" || type == "ceiling")
        {
            addEntity(new Entity(TileSet{testtex,blocks[i]["width"],blocks[i]["height"],{blocks[i]["textureW"],blocks[i]["textureH"]}},blocks[i]["x"],blocks[i]["y"]));
        }
    }
}

void Level::KeyHandling()
{
    if(GetKeyState(VK_LEFT) < 0)
    {
        hero->state = EnttityAnimationState::WALKING;
        hero->side = EnttitySide::LEFT;
        hero->velocity.x += -10000;
        hero->line = 0;
    }
    if(GetKeyState(VK_RIGHT) < 0)
    {
        hero->state = EnttityAnimationState::WALKING;
        hero->side = EnttitySide::RIGHT;
        hero->velocity.x += +10000;
        hero->line = 0;
    }
    if((GetKeyState(VK_UP) & 0x8000))
    {
        if(!hero->inAir && hero->state != EnttityAnimationState::JUMPING)
        {
            hero->state = EnttityAnimationState::JUMPING;
            hero->line = 1;
        }
    }
    if(!(GetKeyState(VK_UP) & 0x8000))
    {
        if(!hero->inAir && hero->state == EnttityAnimationState::JUMPING)
        {
            hero->state = EnttityAnimationState::STAYING;
            hero->velocity.y = 10000;
        }
    }
}

void Level::reSize(float screenW, float screenH)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, screenW, screenH);
    glOrtho(0, screenW, 0, screenH, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Level::update(float elapsedGameTime)
{
    KeyHandling();
    for(Entity* entity : entitys)
    {
        entity->update(elapsedGameTime);
    }
}

void Level::renderAll()
{
    for(Entity* entity : entitys)
    {
        entity->render();
        CollisionManager::draw(CollisionManager::colTable[entity]);
    }

}
