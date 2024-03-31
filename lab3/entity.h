#pragma once
#include "texture.h"
#include <string>

class Entity
{
public:
    void render();
    Entity(Texture texture,float x,float y, float velocity): texture{texture},x{x},y{y},velocity{velocity}{texture.bindTexture();};
    Texture texture;
    void update(float elapsedGameTime);
    float getX() const{return x;};
    float getY() const{return y;};
    float dx = 0;
    float dy = 0;
    float x;
    float y;
    float velocity = 0;
    int currentState = 1;
    bool inAir = false;
    bool isWalking = false;
    bool isJumping = false;
    float getState()const{return currentState;};
    float setState(float state) {currentState = state;};
    void setVel(float velocity){this->velocity = velocity;};
private:
    float vert[8];
    float cord[8] = {0,0, 1,0, 1,1, 0,1};

    float currentTime = 0;
    float period = 0.06;
    int valFrames = 7;
    int valCol = 3;
    int currentFrame = 0;
    float frameWidth=80;
    float frameHeight=80;
};
