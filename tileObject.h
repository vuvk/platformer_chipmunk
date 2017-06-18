#pragma once
#ifndef TILE_OBJECT_H
#define TILE_OBJECT_H


#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "chipmunk/chipmunk_private.h"

#include "defines.h"
#include "list.h"


/* simple tile */
typedef struct
{
    cpVect pos;
    float w, h;
    int texIndex;
} STile;

/* tile object with angles and sizes */
typedef struct
{
    cpVect pos;
    float w, h;
    float angle;

    SDL_Texture* texture;
} STileObject;


// наш уровень состоит массива тайловых объектов,
// которые статичны в пространстве и имеют одиннаковый размер, поворот
// + двух списка для задних и передних объектов
SList* backTiles;
SList* frontTiles;
STile* tiles [LEVEL_HEIGHT][LEVEL_WIDTH];


// рисуем фоновую картинку
void BackgroundDraw();


/* operations with tiles */
STile* TileCreate (int texIndex);
void TileDestroy (STile** tile);
void TileClearAll ();
/*void TileGetSdlRect (STile* tile, SDL_Rect* rect);
SDL_Texture* TileObjectGetTexture (STile* tile);*/

void TilesUpdateAndRender ();


/* operations with tile objects */
STileObject* TileObjectCreate (float x, float y,
                               float w, float h,
                               float angle,
                               SDL_Texture* texture);
void TileObjectDestroy (STileObject** tileObject);
void TileObjectClearAll ();

void TileObjectGetSdlRect (STileObject* tileObject, SDL_Rect* rect);
SDL_Texture* TileObjectGetTexture (STileObject* tileObject);

void TileObjectsUpdateAndRender (SList* tilesList);

#endif // BACKGROUND_OBJECT_H

