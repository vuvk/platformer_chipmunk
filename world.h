#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "chipmunk/chipmunk_private.h"




//bool dinamycSky;
bool drawBB;


static const cpVect upVect    = { 0,  1};
static const cpVect downVect  = { 0, -1};
static const cpVect leftVect  = {-1,  0};
static const cpVect rightVect = { 1,  0};

typedef enum
{
    NO_COLLISION = 0,
    COLLISION_TYPE_ONE_WAY = 1,
    COLLISION_TYPE_ALL
} ECollisionType;


/* physical space */
cpSpace* world;

void WorldCreate();
void WorldDestroy();
void WorldUpdate(float dt);


#endif // WORLD_H
