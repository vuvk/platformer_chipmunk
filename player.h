#pragma once
#ifndef PLAYER_H
#define PLAYER_H


//#include "physBody.h"
#include "creature.h"

SCreature* player;

static const float PLAYER_JUMP_HEIGHT = BLOCK_SIZE * 8.0f;

int16 playerPrevHealth;
bool playerCanDamaged;
bool playerMoveL;
bool playerMoveR;


/**
 -== playerCountJump ==-
    0 - на земле
    1 - был первый прыжок
    2 - двойной прыжок разрешён прыжок
    3 - был второй прыжок, отсекайся!
*/
ubyte playerCountJump;

/*typedef struct
{
    SCreature* creature;
    bool moveL;
    bool moveR;
} SPlayer;*/


/* update player state */
void PlayerUpdate ();
/* apply frames for events*/
void PlayerUpdateFrames ();


#endif // PLAYER_H
