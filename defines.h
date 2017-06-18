#pragma once
#ifndef DEFINES_H
#define DEFINES_H

//#include "vector2d.h"
#include "chipmunk/chipmunk_private.h"

#define GAME_NAME "--== LUNARIANS ==--"

/* main window size */
#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 544

/* level size */
#define LEVEL_WIDTH  100
#define LEVEL_HEIGHT  15

/* background texture size */
#define BACKGROUND_WIDTH  1024
#define BACKGROUND_HEIGHT  768

/* ERRORS */
#define ERR_NONE                      0
#define ERR_SDL_NOT_INITIALIZED       1
#define ERR_SDL_WINDOW_NOT_CREATED    2
#define ERR_SDL_RENDERER_NOT_CREATED  3
#define ERR_SDL_AUDIO_NOT_INITIALIZED 4

#define EPSILON 0.01

/* game logics*/
#define BLOCK_SIZE  64
#define TILE_SIZE  128
#define COUNT_TILES_HORIZONTAL  7
#define COUNT_TILES_VERTICAL   16

/* physics */
/*#define GRAVITY BLOCK_SIZE/3.0f
#define MIN_VERTICAL_IMPULSE -GRAVITY
#define MAX_VERTICAL_IMPULSE  GRAVITY
#define MIN_HORIZONTAL_IMPULSE -GRAVITY
#define MAX_HORIZONTAL_IMPULSE  GRAVITY*/

/* 60 frames per second */
//#define LIMIT_FPS 60
static const float LIMIT_FPS = 1000.0f/60.0f;

/* === NEW TYPES === */

/* booleans */
#define false 0
#define true  1
typedef unsigned char bool;

/* byte */
typedef char byte;
typedef unsigned char ubyte;

/* int16 */
typedef signed short int int16;
typedef unsigned short int uint16;

/* int32 */
typedef signed int int32;
typedef unsigned int uint32;

/* int64 */
typedef signed long int int64;
typedef unsigned long int uint64;



/* === GLOBALS === */
float deltaTime;
uint16 fps;
cpVect cameraPos;


/*#define GRABBABLE_MASK_BIT (1<<31)
cpShapeFilter GRAB_FILTER = {CP_NO_GROUP, GRABBABLE_MASK_BIT, GRABBABLE_MASK_BIT};
cpShapeFilter NOT_GRABBABLE_FILTER = {CP_NO_GROUP, ~GRABBABLE_MASK_BIT, ~GRABBABLE_MASK_BIT};*/


#endif // DEFINES_H
