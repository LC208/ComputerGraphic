#include "entity.h"
#include "const.h"
#include <GL/gl.h>
#include <math.h>
#include <iostream>
#include "collisionmanager.h"

vec2f delta{0,0};

Entity::Entity(TileSet tileset, float x, float y): tileset{tileset},pos{x,y}
{
    this->uid = all_entities++;
}

void Entity::render()
{
    vert[0]=vert[6]=0;
    vert[2]=vert[4]=tileset.tileWidth;
    vert[1]=vert[3]=0;
    vert[5]=vert[7]=tileset.tileHeight;
    float* cord = tileset.getVertOfTile(tile,line,side);
    glPushMatrix();
    glTranslatef(pos.x,pos.y,0);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, this->tileset.texture.textureID);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glTexCoordPointer(2, GL_FLOAT, 0, cord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}



vec2f gravity{0,-9.8f};
vec2f blockReaction{0,9.8f};

float period = 0.08;
float currentTime = 0;
float restitution = 0.5f;

void MoveabelEntity::update(float elapsed_seconds)
{
    for(auto & element : CollisionManager::colTable)
    {
        if(element.first != this)
        {
            HitInfo res = CollisionManager::boxCast(this,element.second ,elapsed_seconds);
            float ProjectedVelocity = vec2<float>::dot(res.normal,velocity);
            vec2f reaction{-(1+restitution),-1.0f};
            velocity+= res.normal * ProjectedVelocity * reaction;
        }
    }
    movementUpdate(elapsed_seconds);
    velocity += gravity;
    //std::cout << res.nearTime << std::endl;
    currentTime+=elapsed_seconds;
    if(currentTime > period)
    {
        currentTime -= period;
        std::array<int,3> lineAndTiles = actionToLineAndTiles[state];
        line = lineAndTiles[0];
        tile < lineAndTiles[1] ? tile++ : lineAndTiles[2] ? tile = 0 : 0;
    }
    glPushMatrix();
    glBegin(GL_LINES);
    glLineWidth(1);
    glVertex2f(pos.x,pos.y);
    glVertex2f(pos.x+velocity.x ,pos.y+velocity.y);
    glEnd();
    glPopMatrix();
}
float movementTime = 0;
float movPeriod = 0.01;

void MoveabelEntity::movementUpdate(float elapsed_seconds)
{

    movementTime+=elapsed_seconds;
    if(movementTime > movPeriod)
    {
        movementTime-=movPeriod;
        vec2f prevpos = pos;
        pos+=velocity*(elapsed_seconds);//pos+=velocity*elapsed_seconds + a*elapsed_seconds*elapsed_seconds*0.5d;
        if(std::abs(pos.y - prevpos.y) > 1)
        {
            inAir = true;
        }
        CollisionManager::colTable[this].pos = pos;

    }
}

void Entity::update(float elapsed_seconds)
{
}
