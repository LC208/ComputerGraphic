#include "collisionmanager.h"
#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <limits>
std::map<Entity*, CollisionBox> CollisionManager::colTable;


void CollisionManager::draw(CollisionBox box)
{
    float vert[8];
    vert[0]=vert[6]=box.pos.x;
    vert[2]=vert[4]=box.pos.x + box.width+1;
    vert[1]=vert[3]=box.pos.y;
    vert[5]=vert[7]=box.pos.y + box.height+1;
    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

vec2f CollisionManager::lineIntersec(vec2f A, vec2f C, vec2f B, vec2f D)
{
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1*(A.x) + b1*(A.y);

    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2*(C.x)+ b2*(C.y);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        return vec2f{-666,-666};
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        if(x <= std::max(C.x,D.x) && x >= std::min(C.x,D.x))
        {
            if(y <= std::max(C.y,D.y) && y >= std::min(C.y,D.y))
            {
                return vec2f(x, y);
            }
        }
        return vec2f(-666, -666);

    }
}

CollisionBox getPhaseBox(MoveabelEntity* entity)
{
  CollisionBox phaseBox;
  CollisionBox b = CollisionManager::colTable[entity];
  phaseBox.pos.x = entity->velocity.x > 0 ? b.pos.x : b.pos.x + entity->velocity.x;
  phaseBox.pos.y = entity->velocity.y > 0 ? b.pos.y : b.pos.y + entity->velocity.y;
  phaseBox.width = entity->velocity.x > 0 ? entity->velocity.x + b.width : b.width - entity->velocity.x;
  phaseBox.height = entity->velocity.y > 0 ? entity->velocity.y + b.height : b.height - entity->velocity.y;

  return phaseBox;
}

bool AABBCheck(CollisionBox b1, CollisionBox b2)
{
  return (std::max(b1.pos.x, b2.pos.x) <= std::min(b1.pos.x + b1.width, b2.pos.x + b2.width)) && (std::max(b1.pos.y, b2.pos.y) <= std::min(b1.pos.y + b1.height, b2.pos.y + b2.height));
}

HitInfo CollisionManager::boxCast(MoveabelEntity* entity,CollisionBox rect ,float elapsed_time)
{
    HitInfo out{vec2f{0,0},0};
    CollisionBox box = CollisionManager::colTable[entity];
    vec2f pos = box.pos;
    CollisionBox phaseBox = getPhaseBox(entity);
    draw(phaseBox);
    if(AABBCheck(phaseBox, rect))
    {
        vec2f normal;
        float time;
        float xNearDist, yNearDist , xNear, yNear;
        float xFarDist, yFarDist, xFar , yFar;
        if(entity->velocity.x > 0)
        {
            xNearDist = rect.pos.x - (box.pos.x + box.width);
            xFarDist = (rect.pos.x + rect.width) - box.pos.x;
        }
        else
        {
            xNearDist = (rect.pos.x + rect.width) - box.pos.x;
            xFarDist =  rect.pos.x  - (box.pos.x + box.width);
        }


        if(entity->velocity.y > 0)
        {
            yNearDist =  rect.pos.y - (box.pos.y + box.height) ;
            yFarDist = (rect.pos.y + rect.height) - box.pos.y;
        }
        else
        {
            yNearDist = (rect.pos.y + rect.height) - box.pos.y;
            yFarDist = rect.pos.y - (box.pos.y + box.height);
        }

        if(entity->velocity.x == 0)
        {
            xNear = -std::numeric_limits<float>::infinity();

            xFar = std::numeric_limits<float>::infinity();
        }
        else
        {
            xNear = xNearDist/entity->velocity.x;
            xFar = xFarDist/entity->velocity.x;
        }

        if(entity->velocity.y == 0)
        {
            yNear = -std::numeric_limits<float>::infinity();
            yFar = std::numeric_limits<float>::infinity();
        }
        else
        {
            yNear = yNearDist/entity->velocity.y;
            yFar = yFarDist/entity->velocity.y;
        }

        //if (yNear > elapsed_time) yNear = -std::numeric_limits<float>::infinity(); // From previous bug above.
        //if (xNear > elapsed_time) xNear = -std::numeric_limits<float>::infinity();

        float nearTime = std::max(xNear,yNear);
        float farTime = std::min(xFar, yFar);
        //std::cout << yNear << " " << xNear <<  std::endl;
        if(nearTime > farTime || ((xNear < 0) && (yNear < 0)) || yNear > elapsed_time || xNear > elapsed_time)
        {
            //падает
            normal= vec2f{0,0};
            time = elapsed_time;
        }
        else
        {
            //стоит на платформе
            //std::cout << yNear << " " << xNear <<  std::endl;
            //entity->velocity += vec2f{0,9.8f};
            entity->inAir = false;
            if(xNear > yNear)
            {
                if(xNearDist < 0)
                {
                    normal= vec2f{1,0};
                }
                else
                {
                    normal= vec2f{-1,0};
                }
            }
            else
            {
                if(yNearDist < 0)
                {
                    normal= vec2f{0,1};
                }
                else
                {
                    normal= vec2f{0,-1};
                }
            }
            time = nearTime;
        }
        out = {normal, time};
    }
    return out;
}

/*
vec2f CollisionManager::lineCast(vec2f prevpos, vec2f pos)
{

    for (auto el : colTable)
    {
        CollisionBox rect = el.second;
        //y
        vec2f y;
        if(pos.y - prevpos.y <= 0)
        {
            vec2f y = lineIntersec(pos,rect.pos+vec2f{0,rect.height},prevpos,rect.pos+vec2f{rect.width,rect.height});
        }
        else
        {
            vec2f y = lineIntersec(pos,rect.pos,prevpos,rect.pos+vec2f{rect.width,0});
        }
        //x
        vec2f x;
        if(pos.x - prevpos.x <= 0)
        {
            vec2f x = lineIntersec(pos,rect.pos+vec2f{rect.width,0},prevpos,rect.pos+vec2f{rect.width,rect.height});
        }
        else
        {
            vec2f x = lineIntersec(pos,rect.pos,prevpos,rect.pos+vec2f{0,rect.height});
        }

        return x.x != -666 ? x : y;
    }
}
*/
