#pragma once
#ifndef CREATURE_H
#define CREATURE_H

#include "SDL2/SDL.h"
#include "chipmunk/chipmunk_private.h"

#include "list.h"
#include "physBody.h"


typedef enum
{
    ctPlayer,
    ctMushroom,
    ctGoomba,
    ctCoopa
} ECreatureType;

typedef struct
{
    ECreatureType creatureType;
    int16 health;

    SPhysBody* physBody;

    char xDir;              // -1 ����� 1 ������
    float moveSpeed;        // �������� �������� ������������
    float accelSpeed;       // ���������

    // ��������
    SDL_Texture** textures;
    ubyte texW, texH;
    uint16 texCount;

    // ��������
    short startFrame, endFrame;
    short curFrame;
    float animSpeed;        // ��� ������, ��� ������� ��������!
    float animDelay;        // ����� ���� ��������� ������� �� ����� �����
} SCreature;

/* ����� ���� �������� */
SList* creatures;

/* �������� ������������ */
SCreature* CreatureCreate (ECreatureType creatureType,
                           short health,
                           float x, float y,
                           ubyte bodyW, ubyte bodyH,
                           float radius,
                           float moveSpeed,
                           float mass,
                           float friction,
                           SDL_Texture** textures,
                           unsigned short texCount,
                           ubyte texW, ubyte texH,
                           float animSpeed);
/* �������� ����������� */
void CreatureDestroy (SCreature** creature);
void CreatureClearAll ();

void CreatureGetDamage (SCreature* creature, int damage);

void CreatureUpdateState (SCreature* creature);

/* ��������� ������ � �������� */
/*void CreatureAddImpulse (SCreature* creature, float x, float y);
void CreatureUpdatePhysics (SCreature* creature);
bool CreatureContainsPoint (SCreature* creature, float x, float y);
bool CreatureIsCollisionCreature (SCreature* c1, SCreature* c2);
bool CreatureIsCollisionLevelObject (SCreature* c1, SLevelObject* l2);*/
/* ��������� �������� */
void CreatureSetFrameRange (SCreature* creature, int startFrame, int endFrame);
void CreatureUpdateAnimation (SCreature* creature);
/* ������� */
void CreatureGetSdlRect (SCreature* creature, SDL_Rect* rect);
SDL_Texture* CreatureGetTexture (SCreature* creature, int numFrame);
/* AI */
void CreatureUpdateAI (SCreature* creature);

void CreaturesUpdateAndRender();


#endif // CREATURE_H
