#include "entity.h"
#include "const.h"
#include <GL/gl.h>
#include <math.h>
#include <iostream>

void Entity::render()
{
    vert[0]=vert[6]=getX();
    vert[2]=vert[4]=getX() + frameWidth;
    vert[1]=vert[3]=getY();
    vert[5]=vert[7]=getY() + frameHeight;
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->texture.textureID);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    cord[1] = cord[3] = (frameHeight*(std::abs(currentState)-2))/240 + (frameHeight/240);
    cord[5] = cord[7] = (frameHeight*(std::abs(currentState)-2))/240;
    cord[2] = cord[4] = (frameWidth*currentFrame)/640 + (currentState > 0 ? 1 : 0)*(frameWidth/640);
    cord[0] = cord[6] = (frameWidth*currentFrame)/640 + (currentState < 0 ? 1 : 0)*(frameWidth/640) ;
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glTexCoordPointer(2, GL_FLOAT, 0, cord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}

void Entity::update(float elapsed_seconds)
{
    int numOfAction = std::abs(currentState);
    if(isWalking)
    {
        dx += (numOfAction/getState())*velocity;
    }
    if(isJumping)
    {
        dy += 500*velocity;
        isJumping = false;
        numOfAction == 3 ? currentState /= numOfAction : 0;
    }
    y >= 10 ? y += dy/8 - 9.8 : y += dy/8;
    y < 10 ? y = 10: 0;
    dy > 0.001 ? dy -= dy/8 : dy = 0;
    x += dx/8;
    std:abs(dx) > 0.001 ? dx -= dx/8 : dx = 0;
    if(numOfAction != 1)
    {
        currentTime+=elapsed_seconds;
        if(currentTime > period)
        {
            currentTime -= period;
            std::cout << currentState << std::endl;
            numOfAction == 3 && currentFrame == valFrames - 2 ? 0 : currentFrame < valFrames ? currentFrame++ : currentFrame = 0;
        }
    }
    else if(numOfAction == 1)
    {
        currentFrame = 0;
    }

}
