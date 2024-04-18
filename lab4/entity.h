#pragma once
#include "tileset.h"
#include <string>
#include "vec2.h"
#include "const.h"
#include <iostream>

class Entity
{
public:
    void render();
    Entity(TileSet tileset,float x,float y);
    TileSet tileset;
    int line = 0;
    int tile = 0;
    virtual void update(float elapsedGameTime);
    vec2f pos;
    EnttitySide side = EnttitySide::RIGHT;
    int getUID(){return uid;};
    bool operator!=(Entity s) {
		return this->getUID() != s.getUID();
	}
	static int all_entities;
private:
    float vert[8];
    int uid;
};

struct HitInfo
{
    vec2f normal;
    float nearTime;
};

class MoveabelEntity: public Entity
{
public:
    void update(float elapsedGameTime) override;
    void movementUpdate(float elapsedGameTime);
    float a = 1;
    short state = EnttityAnimationState::STAYING;
    bool inAir = false;
    std::map<EnttityAnimationState, std::array<int,3>> actionToLineAndTiles;
    MoveabelEntity(TileSet tileset,float x,float y,float width, float height, float a, std::map<EnttityAnimationState, std::array<int,3>> actionToLineAndTiles) : Entity{tileset,x,y}, a{a},actionToLineAndTiles{actionToLineAndTiles}{};
    vec2f velocity{0,0};
};
