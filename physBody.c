#include "physBody.h"


SPhysBody* PhysBodyCreateBox (cpSpace* space,
                              float mass, float radius, float friction, cpFloat moment,
                              float x, float y,
                              float w, float h)
{
    if (space == NULL)
        return NULL;

    SPhysBody* physBody = (SPhysBody*) malloc (sizeof (SPhysBody));

    // момент инерции?
    //cpFloat moment = cpMomentForBox (mass, w, h);

    // создаем тело
    physBody->body = cpSpaceAddBody (space, cpBodyNew (mass, moment));
    cpBodySetPosition (physBody->body, cpv (x, -y));

    // теперь создаем коллизионную форму
    physBody->shape = cpSpaceAddShape (space, cpBoxShapeNew (physBody->body, w, h, radius));
    cpShapeSetFriction (physBody->shape, friction);

    physBody->isGrounded = false;
    /*physBody->w = w;
    physBody->h = h;

    physBody->pos = cpv (x, y);
    physBody->center = cpv (x + w / 2.0f, y + h / 2.0f);*/

    return physBody;
}


SPhysBody* PhysBodyCreateCircle (cpSpace* space,
                                 float mass, float radius, float friction,
                                 float x, float y)
{
    if (space == NULL)
        return NULL;

    SPhysBody* physBody = (SPhysBody*) malloc (sizeof (SPhysBody));

    // момент инерции?
    cpFloat moment = cpMomentForCircle (mass, 0.0f, radius, cpvzero);

    // создаем тело
    physBody->body = cpSpaceAddBody (space, cpBodyNew (mass, moment));
    cpBodySetPosition (physBody->body, cpv(x, y));

    // теперь создаем коллизионную форму
    physBody->shape = cpSpaceAddShape (space, cpCircleShapeNew (physBody->body, radius, cpvzero));
    cpShapeSetFriction (physBody->shape, friction);

    return physBody;
}

void PhysBodyDestroy (SPhysBody** physBody)
{
    if (physBody == NULL || *physBody == NULL)
        return;

    cpShapeFree ((*physBody)->shape);
    cpBodyFree ((*physBody)->body);

    free (*physBody);
    *physBody = NULL;
}

static void SelectGroundNormal (cpBody* body, cpArbiter* arb, cpVect* groundNormal)
{
	cpVect n = cpvneg (cpArbiterGetNormal(arb));

	if (n.y > groundNormal->y)
        *groundNormal = n;
}

void PhysBodyUpdateState (SPhysBody* physBody)
{
    if (physBody == NULL)
        return;

	// Grab the grounding normal from last frame
	cpVect groundNormal = cpvzero;
    cpBodyEachArbiter (physBody->body, (cpBodyArbiterIteratorFunc) SelectGroundNormal, &groundNormal);

    //physBody->isGrounded = (groundNormal.y > 0.0);
    physBody->isGrounded = (groundNormal.y >= EPSILON);
}

void PhysBodyUpdateParameters (SPhysBody* physBody)
{
    if (physBody == NULL)
        return;

    /*cpBB bbRect = cpShapeGetBB (physBody->shape);
    physBody->pos.x =  bbRect.l;
    physBody->pos.y = -bbRect.t;
    physBody->center.x = pos.x + (bbRect.r - bbRect.l) / 2.0f;
    physBody->center.y = pos.y + (bbRect.t - bbRect.b) / 2.0f;*/

    /*cpVect pos = cpBodyGetPosition (physBody->body);
    pos.y = -pos.y;     // ось Y перевёрнута!
    cpVect center = cpv (pos.x + physBody->w / 2.0f, pos.y + physBody->h / 2.0f);

    physBody->pos = pos;
    physBody->center = center;*/
}
