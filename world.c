#include "defines.h"
#include "world.h"
#include "physBody.h"

static cpCollisionHandler* handler;

static cpBool PreSolveOneWayPlatform (cpArbiter* arb, cpSpace* space, void* ignore)
{
	CP_ARBITER_GET_SHAPES (arb, a, b);
	//OneWayPlatform* platform = (OneWayPlatform*) cpShapeGetUserData(a);
	cpVect* n = (cpVect*) cpShapeGetUserData(a);

	if (cpvdot (cpArbiterGetNormal (arb), *n) < EPSILON)
        return cpArbiterIgnore (arb);

	return cpTrue;
}
void WorldCreate()
{
    cpVect gravity = cpv (0.0f, -BLOCK_SIZE*10);

    // ������� ���
    world = cpSpaceNew ();
    cpSpaceSetGravity (world, gravity);

    // ��������� ��������� ������������
    /*cpShape* leftWall = cpSegmentShapeNew (space->staticBody, cpv (0.0f, 0.0f), cpv (0.0f, -450.0f), 10.0f);
    cpShapeSetFriction (leftWall, 1.0f);
    cpSpaceAddShape (space, leftWall);

    cpShape* rightWall = cpSegmentShapeNew (space->staticBody, cpv (620.0f, 0.0f), cpv (620.0f, -450.0f), 10.0f);
    cpShapeSetFriction (rightWall, 1.0f);
    cpSpaceAddShape (space, rightWall);*/


    /* ������� ��� ������������� �������� � ������ COLLISION_TYPE_ONE_WAY */
	cpCollisionHandler* handler = cpSpaceAddWildcardHandler (world, COLLISION_TYPE_ONE_WAY);
	handler->preSolveFunc = PreSolveOneWayPlatform;
}

inline void WorldDestroy()
{
    cpfree (handler);
    cpSpaceFree (world);
}

inline void WorldUpdate (float dt)
{
    cpSpaceStep (world, dt);
}
