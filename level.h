#pragma once
#ifndef LEVEL_H
#define LEVEL_H


#include "chipmunk/chipmunk_private.h"

#include "defines.h"
#include "list.h"
#include "tileObject.h"

// типы объектов уровня
typedef enum
{
    lotNone              = 0,

    /* статичные блоки*/
    lotBlock,                 // квадратный
    lotBlockLeftToUp,         // треугольник - подъем слева вверх
    lotBlockRightToUp,        // треугольник - подъем справа вверх
    lotBlockTrasparentDown    // блок, на который можно запрыгнуть снизу насквозь

} ELevelObjectType;

// объект уровня
typedef struct
{
    //cpVect startPos;                 // позиция, в которой объект был создан
    cpVect pos;                      // текущая позиция
    ELevelObjectType levelObjectType;

    cpBody* body;
    cpShape* shape;

    //int texIndex;
} SLevelObject;



SLevelObject* level [LEVEL_HEIGHT][LEVEL_WIDTH];


// имитация конструктора
SLevelObject* LevelObjectCreate (cpSpace* space,
                                 ELevelObjectType levelObjectType,
                                 float x, float y,
                                 //bool isSolid,
                                 //bool isStatic,
                                 int texIndex);

// имитация деструктора
void LevelObjectDestroy (SLevelObject** levelObject);

// загрузка уровня (наполнение массива level)
void LevelLoad (char* fileName);
void LevelClear ();

void LevelUpdateAndRender ();

#endif // LEVEL_H
