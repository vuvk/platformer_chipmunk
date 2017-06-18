#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL2/SDL.h"

#include "defines.h"
#include "engine.h"

typedef struct
{
    char* fileName;
    int w, h;
} STexture;


/* textures of level */
SDL_Texture* levelTextures;

/* textures of player */
SDL_Texture* playerTextures[8];

/* фон */
SDL_Texture* backgroundTexture;

/* текстуры для фоновых объектов */
SDL_Texture* tileObjectTextures[14];

void TextureLoad (SDL_Texture** texture, const char* fileName);
void LoadTextures();

#define TextureSetColor (texture, r, g, b) TextureArraySetColor (texture, r, g, b, 1)
/* можно указать массив для установки цвета всем элементам */
void TextureArraySetColor (SDL_Texture** textures, const ubyte r, const ubyte g, const ubyte b, uint16 count);


#endif // TEXTURE_H
