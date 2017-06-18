#pragma once
#ifndef ENGINE_H
#define ENGINE_H


#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "chipmunk/chipmunk_private.h"

#include "defines.h"
//#include "vector2d.h"


bool EngineStart();
void EngineStop();

bool EngineCreateWindow (const char* title, int x, int y, int w, int h, unsigned long flags);
void EngineSetWindowTitle (const char* title);
bool EngineCreateRenderer (int index, unsigned long flags);
bool EngineInitAudio();
SDL_Texture* EngineLoadTexture (const char* fileName);
//SDL_Texture* EngineLoadTextureFromStrip (SDL_Texture* srcTexture, const SDL_Rect* srcRect);


void EngineRenderClear();
void EngineRenderLine (int x1, int y1, int x2, int y2, ubyte r, ubyte g, ubyte b, ubyte a);
void EngineRenderImage (SDL_Texture* texture, const SDL_Rect* rect, bool flipX);
void EngineRenderImageEx (SDL_Texture* texture, const SDL_Rect* rect,
                          const double angle,
                          //const SVector2f* center,
                          const cpVect* center,
                          const SDL_RendererFlip flip);
void EngineRenderTile (SDL_Texture* tiles, const SDL_Rect* srcRect, const SDL_Rect* dstRect);

void EngineRenderPresent();
void EngineUpdateTime();

void EngineClearAllInstances();


#endif // ENGINE_H
