
#include "additions.h"
#include "engine.h"
#include "texture.h"
#include "tileObject.h"
#include "level.h"


void BackgroundDraw ()
{
    SDL_Rect srcRect, dstRect;

    /* draw background first */
    srcRect.w = WINDOW_WIDTH;
    srcRect.h = WINDOW_HEIGHT;

    /** !!!!!! */
    srcRect.x = (int) ceil (cameraPos.x / (LEVEL_WIDTH  * BLOCK_SIZE) * (BACKGROUND_WIDTH - srcRect.w));
    /*if (!dinamycSky)
        srcRect.y = BACKGROUND_HEIGHT - srcRect.h;
    else*/
        srcRect.y = BACKGROUND_HEIGHT - srcRect.h - (LEVEL_HEIGHT * BLOCK_SIZE - WINDOW_HEIGHT - (int)ceil(cameraPos.y));

    LimitInt (&(srcRect.x), 0, BACKGROUND_WIDTH  - srcRect.w);
    LimitInt (&(srcRect.y), 0, BACKGROUND_HEIGHT - srcRect.h);

    dstRect.x = dstRect.y = 0;
    dstRect.w = WINDOW_WIDTH;
    dstRect.h = WINDOW_HEIGHT;
    EngineRenderTile (backgroundTexture, &srcRect, &dstRect);
}


/***********************/
/******** TILES ********/
/***********************/

STile* TileCreate (int texIndex)
{
    STile* tile = (STile*) malloc(sizeof (STile));
    /*tile->pos = cpv (x, y);
    tile->w = w;
    tile->h = h;*/
    tile->texIndex = texIndex;

    return tile;
}

void TileDestroy (STile** tile)
{
    if (tile == NULL || *tile == NULL)
        return;

    free (*tile);
    *tile = NULL;
}

void TileClearAll ()
{
    for (uint32 r = 0; r < LEVEL_HEIGHT; r++)
        for (uint32 c = 0; c < LEVEL_WIDTH; c++)
            if (tiles [r][c] != NULL)
                TileDestroy ((STile**) &(tiles[r][c]));
}

/*void TileGetSdlRect (STile* tile, SDL_Rect* rect)
{
    if (tile == NULL || rect == NULL)
        return;

    rect->x = (int) ceil (tile->pos.x - cameraPos.x);
    rect->y = (int) ceil (tile->pos.y - cameraPos.y);
    rect->w = (int) ceil (tile->w);
    rect->h = (int) ceil (tile->h);
}

inline SDL_Texture* TileObjectGetTexture (STile* tile)
{
    if (tile == NULL)
        return NULL;
}*/

void TilesUpdateAndRender ()
{
    /* ограничиваем обрабатываемые блоки видимостью камеры */
    int16 viewColMin = (int16)(floor(cameraPos.x)) / BLOCK_SIZE;
    int16 viewColMax = viewColMin + WINDOW_WIDTH / BLOCK_SIZE;
    LimitShort (&viewColMin, 0, LEVEL_WIDTH - 1);
    LimitShort (&viewColMax, 0, LEVEL_WIDTH - 1);

    int16 viewRowMin = (int16)(floor(cameraPos.y)) / BLOCK_SIZE;
    int16 viewRowMax = viewRowMin + WINDOW_HEIGHT / BLOCK_SIZE;
    LimitShort (&viewRowMin, 0, LEVEL_HEIGHT - 1);
    LimitShort (&viewRowMax, 0, LEVEL_HEIGHT - 1);

    SDL_Rect srcRect, dstRect;
    srcRect.x = srcRect.y = 0;
    srcRect.w = srcRect.h = TILE_SIZE;
    dstRect.x = dstRect.y = 0;
    dstRect.w = dstRect.h = BLOCK_SIZE;

    STile* tile;
    for (uint16 r = viewRowMin; r <= viewRowMax; r ++)
    {
        for (uint16 c = viewColMin; c <= viewColMax; c++)
        {
            tile = tiles [r][c];

            if (tile != NULL)
            {
                // update levelObject position...
                /*levelObject->center.x = levelObject->pos.x + (BLOCK_SIZE >> 1);
                levelObject->center.y = levelObject->pos.y + (BLOCK_SIZE >> 1);

                float distY = levelObject->pos.y - levelObject->startPos.y;
                if (abs (distY) > EPSILON)
                {
                    if (levelObject->pos.y < levelObject->startPos.y)
                        levelObject->pos.y += deltaTime*BLOCK_SIZE;

                    if (levelObject->pos.y > levelObject->startPos.y)
                        levelObject->pos.y -= deltaTime*BLOCK_SIZE;
                }
                else
                    if (distY != 0.0f)
                        levelObject->pos.y = levelObject->startPos.y;*/

                //dstRect.x = (int)ceil(levelObject->pos.x - cameraPos.x);
                //dstRect.y = (int)ceil(levelObject->pos.y - cameraPos.y);
                dstRect.x = c * BLOCK_SIZE - (int)ceil(cameraPos.x);
                dstRect.y = r * BLOCK_SIZE - (int)ceil(cameraPos.y);

                /* if rect in screen range */
                if ((dstRect.x + dstRect.w > 0) &&
                    (dstRect.x <= WINDOW_WIDTH) &&
                    (dstRect.y + dstRect.h > 0) &&
                    (dstRect.y <= WINDOW_HEIGHT))
                {
                    srcRect.x = (tile->texIndex % COUNT_TILES_HORIZONTAL) * TILE_SIZE;
                    srcRect.y = (tile->texIndex / COUNT_TILES_HORIZONTAL) * TILE_SIZE;

                    EngineRenderTile (levelTextures, &srcRect, &dstRect);
                }
            }
        }
    }
}




/**********************/
/**** TILE OBJECTS ****/
/**********************/
STileObject* TileObjectCreate (float x, float y,
                                           float w, float h,
                                           float angle,
                                           SDL_Texture* texture)
{
    STileObject* tileObject = (STileObject*) malloc (sizeof (STileObject));

    tileObject->pos = cpv (x, y);
    tileObject->w = w;
    tileObject->h = h;
    tileObject->angle = angle;
    tileObject->texture = texture;

    return tileObject;
}

void TileObjectDestroy (STileObject** tileObject)
{
    if (tileObject == NULL || *tileObject == NULL)
        return;

    free (*tileObject);
    *tileObject = NULL;
}

void TileObjectClearAll ()
{
    // clear background tiles
    for (SListElement* element = backTiles->first; element; element = element->next)
    {
        if (element->value != NULL)
            TileObjectDestroy ((STileObject**) &(element->value));
    }
    ListClear (backTiles);

    // clear front tiles
    for (SListElement* element = frontTiles->first; element; element = element->next)
    {
        if (element->value != NULL)
            TileObjectDestroy ((STileObject**) &(element->value));
    }
    ListClear (frontTiles);
}

void TileObjectGetSdlRect (STileObject* tileObject, SDL_Rect* rect)
{
    if (tileObject == NULL || rect == NULL)
        return;

    rect->x = (int)ceil(tileObject->pos.x - cameraPos.x);
    rect->y = (int)ceil(tileObject->pos.y - cameraPos.y);
    rect->w = (int)ceil(tileObject->w);
    rect->h = (int)ceil(tileObject->h);
}

inline SDL_Texture* TileObjectGetTexture (STileObject* tileObject)
{
    if (tileObject == NULL)
        return NULL;

    return tileObject->texture;
}

void TileObjectsUpdateAndRender (SList* tilesList)
{
    if ((tilesList == NULL) || (tilesList->first == NULL))
        return;

    /* for drawing */
    SDL_Rect rect;
    cpVect center;

    STileObject* tileObject;
    SListElement* element = tilesList->first;
    while (element != NULL)
    {
        tileObject = NULL;
        if (element)
            tileObject = (STileObject*) element->value;

        if (tileObject != NULL)
        {
            TileObjectGetSdlRect (tileObject, &rect);

            // if tile object visible in screen
            if ((rect.x + rect.w >= 0) &&
                (rect.y + rect.y >= 0) &&
                (rect.x <= WINDOW_WIDTH) &&
                (rect.y <= WINDOW_HEIGHT))
            {
                if (tileObject->angle != 0.0f)
                {
                    center = cpv (0, rect.h);
                    EngineRenderImageEx (TileObjectGetTexture (tileObject), &rect, tileObject->angle, &center, false);
                }
                else
                    EngineRenderImage (TileObjectGetTexture (tileObject), &rect, false);
            }
        }

        element = element->next;
    }
}


