#include <stdlib.h>
#include <string.h>

#include "libxml/parser.h"

#include "additions.h"
#include "texture.h"
#include "engine.h"
#include "world.h"
#include "level.h"

//#include "corpse.h"
//#include "lump.h"
//#include "surprise.h"
#include "physBody.h"
#include "tileObject.h"
#include "creature.h"
#include "player.h"


extern float deltaTime;
extern cpVect cameraPos;

/* это число, которое нужно вычесть из gid фонового объекта,        */
/* чтобы получить номер текстуры в массиве текстур фоновых объектов */
static int firstBackObjectsGid;


SLevelObject* LevelObjectCreate (cpSpace* space,
                                 ELevelObjectType levelObjectType,
                                 float x, float y,
                                 //bool isSolid,
                                 //bool isStatic,
                                 int texIndex)
{
    SLevelObject* levelObject = (SLevelObject*) malloc(sizeof(SLevelObject));
    //levelObject->startPos.x = x;
    //levelObject->startPos.y = y;
    levelObject->pos.x = x;
    levelObject->pos.y = y;
    levelObject->levelObjectType = levelObjectType;

    cpBody* body = cpSpaceAddBody (space, cpBodyNewStatic());
    cpBodySetPosition (body, cpv (x + (BLOCK_SIZE >> 1), -y - (BLOCK_SIZE >> 1)));

    cpShape* shape;
    switch (levelObjectType)
    {
        // подъем слева вверх
        case lotBlockLeftToUp :
        {
            shape = cpSpaceAddShape (space, cpSegmentShapeNew (body, cpv (-(BLOCK_SIZE >> 1), -(BLOCK_SIZE >> 1)), cpv((BLOCK_SIZE >> 1), (BLOCK_SIZE >> 1)), 0));
            break;
        }

        // подъем справа вверх
        case lotBlockRightToUp :
        {
            shape = cpSpaceAddShape (space, cpSegmentShapeNew (body, cpv ((BLOCK_SIZE >> 1), -(BLOCK_SIZE >> 1)), cpv(-(BLOCK_SIZE >> 1), (BLOCK_SIZE >> 1)), 0));
            break;
        }

        // низ прозрачный (можно запрыгнуть)
        case lotBlockTrasparentDown :
        {
            shape = cpSpaceAddShape (space, cpSegmentShapeNew (body, cpv (-(BLOCK_SIZE >> 1), (BLOCK_SIZE >> 1)), cpv((BLOCK_SIZE >> 1), (BLOCK_SIZE >> 1)), BLOCK_SIZE/32.0f));
            cpShapeSetCollisionType (shape, COLLISION_TYPE_ONE_WAY);
            cpShapeSetUserData (shape, (cpDataPointer) &upVect);

            break;
        }

        default :
        {
            shape = cpSpaceAddShape (space, cpBoxShapeNew (body, BLOCK_SIZE, BLOCK_SIZE, 0));
            break;
        }
    }

	//cpShapeSetElasticity (shape, 0.0f);
	cpShapeSetFriction (shape, 0.1f);
	//cpShapeSetFilter (shape, NOT_GRABBABLE_FILTER);

    levelObject->body = body;
    levelObject->shape = shape;

    //levelObject->texIndex = texIndex;

    return levelObject;
}

void LevelObjectDestroy (SLevelObject** levelObject)
{
    if ((levelObject != NULL) && (*levelObject != NULL))
    {
        cpBodyFree ((*levelObject)->body);
        cpShapeFree ((*levelObject)->shape);

        free (*levelObject);
        *levelObject = NULL;
    }
}

void LevelClear ()
{
    for (int r = 0; r < LEVEL_HEIGHT; r++)
    {
        for (int c = 0; c < LEVEL_WIDTH; c++)
        {
            if ((level[r][c]) != NULL)
                LevelObjectDestroy (&(level[r][c]));
        }
    }
}

/* парсинг и поиск объектов */
void XmlParsing (xmlNode* start_node, int lvl)
{
    if (start_node)
    {
        xmlNode* node;
        for (node = start_node; node; node = node->next)
        {
            //if (strcmp ((char*) node->name, "text") != 0)
            //if (node->name)
            if (StrEqual ((char*) node->name, "text") == false)
            {
                // считываем gid
                //if (strcmp ((char*) node->name, "tileset") == 0)
                if (StrEqual ((char*) node->name, "tileset") == true)
                {
                    char* name = (char*) xmlGetProp (node, (xmlChar*) "name");
                    //if ((name != NULL) && (strcmp (name, "tile_object_sprites") == 0))
                    if (StrEqual (name, "tile_object_sprites") == true)
                    {
                        char* gidStr = (char*) xmlGetProp (node, (xmlChar*) "firstgid");
                        firstBackObjectsGid = atoi (gidStr);

                        // теперь надо добавить к этому первый id из дочернего tile
                        xmlNode* tileNode = node->children->next;
                        gidStr = (char*) xmlGetProp (tileNode, (xmlChar*) "id");

                        firstBackObjectsGid += atoi (gidStr);

                        //xmlFreeNode (tileNode);
                        free (gidStr);
                        gidStr = NULL;
                    }

                    free (name);
                    name = NULL;
                }

                // level objects
                //if (strcmp ((char*) node->name, "data") == 0)
                if (StrEqual ((char*) node->name, "data") == true)
                {
                    // номера тайлов представленные в формате csv
                    char* csv = (char*) xmlNodeGetContent(node);
                    //printf("%s", csv);

                    uint16 numberOfBlock = 0;
                    char* pch = strtok (csv, ",.-; ");
                    int textureIndex;
                    while (pch != NULL)
                    {
                        textureIndex = atoi (pch) - 1;
                        if (textureIndex > -1)
                        {
                            float x = (numberOfBlock % LEVEL_WIDTH) * BLOCK_SIZE;
                            float y = (numberOfBlock / LEVEL_WIDTH) * BLOCK_SIZE;

                            SLevelObject* levelObject;

                            switch (textureIndex)
                            {
                                // наклонная слева вверх
                                case 21 : case 36 : case 51 : case 66 : case 81 : case 96 :
                                {
                                    levelObject = LevelObjectCreate (world, lotBlockLeftToUp, x, y, textureIndex);
                                    break;
                                }

                                // наклонная справа вверх
                                case 28 : case 43 : case 58 : case 73 : case 88 : case 103 :
                                {
                                    levelObject = LevelObjectCreate (world, lotBlockRightToUp, x, y, textureIndex);
                                    break;
                                }

                                // блоки, на которые можно запрыгнуть снизу
                                case  35 : case  42 : case  49 : case  56 :
                                case  50 : case  57 : case  64 : case  71 :
                                case  65 : case  72 : case  79 : case  86 :
                                case  80 : case  87 : case  94 : case 101 :
                                case  95 : case 102 : case 109 : case   5 :
                                case 110 : case   6 : case  13 : case  20 :
                                {
                                    levelObject = LevelObjectCreate (world, lotBlockTrasparentDown, x, y, textureIndex);
                                    break;
                                }

                                default :
                                {
                                    levelObject = LevelObjectCreate (world, lotBlock, x, y, textureIndex);
                                    break;
                                }
                            }

                            level[numberOfBlock / LEVEL_WIDTH][numberOfBlock % LEVEL_WIDTH] = levelObject;

                            tiles[numberOfBlock / LEVEL_WIDTH][numberOfBlock % LEVEL_WIDTH] = TileCreate (textureIndex);
                        }

                        pch = strtok (NULL, ",.-; ");
                        numberOfBlock++;
                    }

                    free (csv);
                    free (pch);
                    csv = NULL;
                    pch = NULL;
                }

                // creatures or another objects
                //if (strcmp ((char*) node->name, "object") == 0)
                if (StrEqual ((char*) node->name, "object") == true)
                {
                    char* name = (char*) xmlGetProp (node, (xmlChar*) "name");
                    char* xStr = (char*) xmlGetProp (node, (xmlChar*) "x");
                    char* yStr = (char*) xmlGetProp (node, (xmlChar*) "y");
                    char* angleStr = (char*) xmlGetProp (node, (xmlChar*) "rotation");
                    char* wStr = (char*) xmlGetProp (node, (xmlChar*) "width");
                    char* hStr = (char*) xmlGetProp (node, (xmlChar*) "height");

                    // разница между размером тайла в файле и размером отображаемым
                    float sizeCoeff = (float) BLOCK_SIZE / TILE_SIZE;

                    // координаты в tmx странные: позиция по X,Y это левый нижний угол,
                    // а для Бурундука нужна координата центра %)
                    float w = atof(wStr) * sizeCoeff;
                    float h = atof(hStr) * sizeCoeff;
                    //float x = atof(xStr) * sizeCoeff + w / 2.0f;
                    //float y = atof(yStr) * sizeCoeff - h / 2.0f;
                    float x = atof(xStr) * sizeCoeff;
                    float y = atof(yStr) * sizeCoeff;
                    float angle = atof (angleStr);

                    //printf ("pos X : %f\npos Y : %f\n", x, y);
                    //printf ("pos W : %f\npos H : %f\n", w, h);

                    if (name != NULL)
                    {
                        // PLAYER
                        //if (strcmp (name, "player") == 0)
                        if (StrEqual (name, "player") == true)
                        {
                            ubyte bodyW = (ubyte) ceil( 68 * sizeCoeff);
                            ubyte bodyH = (ubyte) ceil(128 * sizeCoeff);
                            ubyte texW  = (ubyte) ceil(128 * sizeCoeff);
                            ubyte texH  = (ubyte) ceil(158 * sizeCoeff);

                            x += bodyW / 2;
                            y -= bodyH / 2;

                            SCreature* creature = CreatureCreate (ctPlayer,                   // type
                                                                  1,                          // health
                                                                  x, y,                       // position
                                                                  bodyW, bodyH,               // body size
                                                                  0.0f,                       // radius
                                                                  BLOCK_SIZE * 5.0f,          // movement speed
                                                                  100.0f,                     // mass
                                                                  1.0f,                       // friction
                                                                  &playerTextures[0], 7,      // textures and number of textures
                                                                  texW, texH,                 // textures size
                                                                  0.1f);                      // speed of animation
                            // link for player
                            player = creature;
                            ListAddElement (creatures, creature);

                            playerCanDamaged = true;
                            playerPrevHealth = 1;
                            TextureArraySetColor (player->textures, 255, 255, 255, player->texCount);
                        }

                        // GOOMBA
                        /*if (strcmp (name, "goomba") == 0)
                        {
                            SCreature* creature = CreatureCreate (ctGoomba,                   // type
                                                              1,                          // health
                                                              x, y,                       // position
                                                              BLOCK_SIZE, BLOCK_SIZE,     // size
                                                              2.5f,                       // movement speed
                                                              &goombaTextures[0], 2,      // textures and number of textures
                                                              0.1f);                      // speed of animation
                            ListAddElement (creatures, creature);
                        }*/



                        // ======================================
                        // ======== BACKGROUND OBJECTS ==========
                        // ======================================
                        //if ((strcmp (name, "backObject") == 0) ||
                        //    (strcmp (name, "frontObject") == 0))

                        if (StrEqual (name, "backObject" ) == true ||
                            StrEqual (name, "frontObject") == true)
                        {
                            char* texIndexStr = (char*) xmlGetProp (node, (xmlChar*) "gid");
                            int texIndex = atoi (texIndexStr) - firstBackObjectsGid;
                            printf ("type is %s\n", texIndexStr);

                            STileObject* tileObject =
                                               TileObjectCreate (x, y - h,                    // position
                                                                       w, h,                        // size
                                                                       angle,
                                                                       tileObjectTextures[texIndex]); // texture

                            if (StrEqual (name, "backObject" ) == true)
                                ListAddElement (backTiles, tileObject);
                            if (StrEqual (name, "frontObject" ) == true)
                                ListAddElement (frontTiles, tileObject);

                            free (texIndexStr);
                            texIndexStr = NULL;
                        }
                    }

                    // free all shit
                    free (name);
                    free (xStr);
                    free (yStr);
                    free (angleStr);
                    free (wStr);
                    free (hStr);

                    name = NULL;
                    xStr = NULL;
                    yStr = NULL;
                    angleStr = NULL;
                    wStr = NULL;
                    hStr = NULL;
                }

                // search all children node, if exists
                if (node->children)
                    XmlParsing(node->children, lvl + 1);
            }
        }

        xmlFreeNode (node);
        node = NULL;
    }
}

/* пробегаемся по всем объектам уровня и объединяем фигуры */
static void LevelOptimize (cpSpace* space)
{
    SLevelObject* levelObject = NULL;
    ELevelObjectType testObjectType;    // тип объекта для тестов
    uint16 r, c;

    // массив с масками, где можно ещё произвести объединения
    bool mask [LEVEL_HEIGHT][LEVEL_WIDTH];
    // где что-то есть - true
    for (r = 0; r < LEVEL_HEIGHT; r++)
        for (c = 0; c < LEVEL_WIDTH; c++)
            mask [r][c] = (level [r][c] != NULL);

    for (r = 0; r < LEVEL_HEIGHT; r++)
    {
        for (c = 0; c < LEVEL_WIDTH; c++)
        {
            if (level [r][c] == NULL)
                continue;

            // запоминаем тип объекта для тестов
            testObjectType = level [r][c]->levelObjectType;

            // объединяем по вертикали все однородные объекты
            // сначала пробежимся и удалим "лишние" физические тела - те, которые повторяют тип тестового
            if (( mask [r][c] == true) &&
                //(level [r][c] != NULL) &&
                (level [r][c]->body != NULL) &&
                (level [r][c]->levelObjectType != lotBlockTrasparentDown) &&
                (level [r][c]->levelObjectType == testObjectType) &&

                (r < LEVEL_HEIGHT - 1) &&
                ( mask [r + 1][c] == true) &&
                (level [r + 1][c] != NULL) &&
                (level [r + 1][c]->body != NULL) &&
                (level [r + 1][c]->levelObjectType == testObjectType))
            {
                // за тестируемым блоком есть такой же, как и он, значит можно сделать один длинный
                uint16 tR = r;   // test row
                bool founded = true;
                // пока что-то находишь, удаляй всё нафиг
                while (founded && (tR < LEVEL_HEIGHT))
                {
                    levelObject = level [tR][c];
                    if ((levelObject == NULL) ||
                        (levelObject->levelObjectType != testObjectType) ||
                        (levelObject->body == NULL) ||
                        (levelObject->shape == NULL) ||
                        (mask [tR][c] == false))
                    {
                        founded = false;
                        continue;
                    }

                    //cpfree (levelObject->shape->userData);
                    levelObject->shape->userData = NULL;
                    levelObject->shape->type = NO_COLLISION;
                    cpSpaceRemoveShape (space, levelObject->shape);
                    cpShapeFree (levelObject->shape);

                    cpSpaceRemoveBody (space, levelObject->body);
                    cpBodyFree (levelObject->body);

                    levelObject->body = NULL;
                    levelObject->shape = NULL;

                    // чистим маску
                    mask [tR][c] = false;

                    tR++;
                }

                // определяем длину повторений
                uint16 len = tR - r;
                // если блоков было больше, чем один, то значит можно сделать один длинный полноценный
                if (len > 1)
                {
                    levelObject = level [r][c];

                    float x = levelObject->pos.x;
                    float y = levelObject->pos.y;

                    cpBody* body = cpSpaceAddBody (space, cpBodyNewStatic());
                    cpBodySetPosition (body, cpv (x + (BLOCK_SIZE >> 1), -y - ((len * BLOCK_SIZE) >> 1)));

                    cpShape* shape = cpSpaceAddShape (space, cpBoxShapeNew (body, BLOCK_SIZE, len * BLOCK_SIZE, 0));

                    levelObject->body = body;
                    levelObject->shape = shape;
                }

                continue;
            }

            // Окей, теперь мы готовы объединять по горизонтали все однородные объекты
            // сначала пробежимся и удалим "лишние" физические тела - те, которые повторяют тип тестового
            if (( mask [r][c] == true) &&
                //(level [r][c] != NULL) &&
                (level [r][c]->body != NULL) &&
                (level [r][c]->levelObjectType == testObjectType) &&

                (c < LEVEL_WIDTH - 1) &&
                ( mask [r][c + 1] == true) &&
                (level [r][c + 1] != NULL) &&
                (level [r][c + 1]->body != NULL) &&
                (level [r][c + 1]->levelObjectType == testObjectType))
            {
                // за тестируемым блоком есть такой же, как и он, значит можно сделать один длинный
                uint16 tC = c;   // test column
                bool founded = true;
                // пока что-то находишь, удаляй всё нафиг
                while (founded && (tC < LEVEL_WIDTH))
                {
                    levelObject = level [r][tC];
                    if ((levelObject == NULL) ||
                        (levelObject->levelObjectType != testObjectType) ||
                        (levelObject->body == NULL) ||
                        (levelObject->shape == NULL) ||
                        (mask [r][tC] == false))
                    {
                        founded = false;
                        continue;
                    }

                    //cpfree (levelObject->shape->userData);
                    levelObject->shape->userData = NULL;
                    levelObject->shape->type = NO_COLLISION;
                    cpSpaceRemoveShape (space, levelObject->shape);
                    cpShapeFree (levelObject->shape);

                    cpSpaceRemoveBody (space, levelObject->body);
                    cpBodyFree (levelObject->body);

                    levelObject->body = NULL;
                    levelObject->shape = NULL;

                    // чистим маску
                    mask [r][tC] = false;

                    tC++;
                }

                // определяем длину повторений
                uint16 len = tC - c;
                // если блоков было больше, чем один, то значит можно сделать один длинный полноценный
                if (len > 1)
                {
                    levelObject = level [r][c];

                    float x = levelObject->pos.x;
                    float y = levelObject->pos.y;

                    cpBody* body = cpSpaceAddBody (space, cpBodyNewStatic());
                    cpBodySetPosition (body, cpv (x + ((len*BLOCK_SIZE) >> 1), -y - (BLOCK_SIZE >> 1)));

                    cpShape* shape;
                    switch (testObjectType)
                    {
                        case lotBlockTrasparentDown :
                        {
                            shape = cpSpaceAddShape (space, cpSegmentShapeNew (body, cpv (-((len * BLOCK_SIZE) >> 1), (BLOCK_SIZE >> 1)), cpv(((len * BLOCK_SIZE) >> 1), (BLOCK_SIZE >> 1)), BLOCK_SIZE/32.0f));
                            cpShapeSetCollisionType (shape, COLLISION_TYPE_ONE_WAY);
                            cpShapeSetUserData (shape, (cpDataPointer) &upVect);

                            break;
                        }

                        default :
                        {
                            shape = cpSpaceAddShape (space, cpBoxShapeNew (body, len * BLOCK_SIZE, BLOCK_SIZE, 0));
                            break;
                        }
                    }

                    levelObject->body = body;
                    levelObject->shape = shape;
                }

                continue;
            }


        }
    }
}

void LevelLoad (char* fileName)
{
    LevelClear();
    TileClearAll();
    TileObjectClearAll(frontTiles);
    TileObjectClearAll(backTiles);

    firstBackObjectsGid = 0;

    if (!FileExists (fileName))
    {
        printf ("File '%s' doesn't exists!\n", fileName);
        return;
    }

    // start parsing xml-level
    xmlDoc* document;
    xmlNode* root;

    document = xmlReadFile (fileName, NULL, 0);
    root = xmlDocGetRootElement (document);

    XmlParsing (root->children, 1);

    //xmlFreeNode (root);
    xmlFreeDoc (document);
    printf ("Level loaded!\n");

    root = NULL;
    document = NULL;

    LevelOptimize (world);
}

void LevelUpdateAndRender ()
{
    uint16 r, c;
    SDL_Rect srcRect, dstRect;
    SDL_Rect rect2;
    SLevelObject* levelObject;

    /* ограничиваем обрабатываемые блоки видимостью камеры */
    int16 viewColMin = (int16)(floor(cameraPos.x)) / BLOCK_SIZE;
    int16 viewColMax = viewColMin + WINDOW_WIDTH / BLOCK_SIZE;
    LimitShort (&viewColMin, 0, LEVEL_WIDTH - 1);
    LimitShort (&viewColMax, 0, LEVEL_WIDTH - 1);

    int16 viewRowMin = (int16)(floor(cameraPos.y)) / BLOCK_SIZE;
    int16 viewRowMax = viewRowMin + WINDOW_HEIGHT / BLOCK_SIZE;
    LimitShort (&viewRowMin, 0, LEVEL_HEIGHT - 1);
    LimitShort (&viewRowMax, 0, LEVEL_HEIGHT - 1);


    srcRect.x = srcRect.y = 0;
    srcRect.w = srcRect.h = TILE_SIZE;
    dstRect.x = dstRect.y = 0;
    dstRect.w = dstRect.h = BLOCK_SIZE;

    for (r = viewRowMin; r <= viewRowMax; r ++)
    {
        for (c = viewColMin; c <= viewColMax; c++)
        {
            levelObject = level [r][c];

            if (levelObject != NULL)
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

                /*dstRect.x = (int)ceil(levelObject->pos.x - cameraPos.x);
                dstRect.y = (int)ceil(levelObject->pos.y - cameraPos.y);

                // if rect in screen range
                if ((dstRect.x + dstRect.w > 0) &&
                    (dstRect.x <= WINDOW_WIDTH) &&
                    (dstRect.y + dstRect.h > 0) &&
                    (dstRect.y <= WINDOW_HEIGHT))
                {
                    if (!drawBB)
                    {
                        srcRect.x = (levelObject->texIndex % COUNT_TILES_HORIZONTAL) * TILE_SIZE;
                        srcRect.y = (levelObject->texIndex / COUNT_TILES_HORIZONTAL) * TILE_SIZE;

                        EngineRenderTile (levelTextures, &srcRect, &dstRect);
                    }
                    else
                        // рисуем bounding box
                        if (levelObject->shape != NULL)
                        {
                            //cpVect pos =  levelObject->body->p;
                            cpBB bbRect = cpShapeGetBB(levelObject->shape);

                            float x = bbRect.l;
                            float y = abs(bbRect.t);
                            float w = bbRect.r - bbRect.l;
                            float h = abs(bbRect.b) - abs(bbRect.t);

                            rect2.x = (int)(x - cameraPos.x);
                            rect2.y = (int)(y - cameraPos.y);
                            rect2.w = (int)(w);
                            rect2.h = (int)(h);

                            EngineRenderLine (rect2.x,           rect2.y,           rect2.x + rect2.w, rect2.y,           0xFF, 0x00, 0x00, 0xFF);
                            EngineRenderLine (rect2.x + rect2.w, rect2.y,           rect2.x + rect2.w, rect2.y + rect2.h, 0xFF, 0x00, 0x00, 0xFF);
                            EngineRenderLine (rect2.x,           rect2.y + rect2.h, rect2.x + rect2.w, rect2.y + rect2.h, 0xFF, 0x00, 0x00, 0xFF);
                            EngineRenderLine (rect2.x,           rect2.y,           rect2.x,           rect2.y + rect2.h, 0xFF, 0x00, 0x00, 0xFF);
                        }
                }*/
            }
        }
    }
}
