#pragma once
#include "vec2.h"
#include <map>
#include "entity.h"

class CollisionBox
{
public:
    float width{0};
    float height{0};
    vec2f pos{0,0};
    CollisionBox(float width, float height, vec2f pos):width{width}, height{height}, pos{pos}{};
    CollisionBox(){};
};

class CollisionManager
{
public:
    static HitInfo boxCast(MoveabelEntity*,CollisionBox ,float);
    static std::map <Entity* ,CollisionBox> colTable;
    //static vec2f lineCast(vec2f prevpos, vec2f pos);
    static void draw(CollisionBox);
    static vec2f lineIntersec(vec2f,vec2f,vec2f,vec2f);
};




