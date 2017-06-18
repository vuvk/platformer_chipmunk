#pragma once
#ifndef PHYSBODY_H
#define PHYSBODY_H

#include "chipmunk/chipmunk_private.h"

#include "defines.h"


typedef struct
{
    cpBody* body;
    cpShape* shape;

    //float w, h;
    bool isGrounded;

    /*cpVect pos;
    cpVect center;*/
} SPhysBody;


SPhysBody* PhysBodyCreateBox (cpSpace* space,
                              float mass, float radius, float friction, cpFloat moment,
                              float x, float y,
                              float w, float h);
SPhysBody* PhysBodyCreateCircle (cpSpace* space,
                                 float mass, float radius, float friction,
                                 float x, float y);
void PhysBodyDestroy (SPhysBody** physBody);

void PhysBodyUpdateState (SPhysBody* physBody);
void PhysBodyUpdateParameters (SPhysBody* physBody);


#endif // PHYSBODY_H
