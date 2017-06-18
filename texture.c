#include <stdio.h>

#include "additions.h"
#include "texture.h"



void TextureLoad (SDL_Texture** texture, const char* fileName)
{
    if (FileExists (fileName))
        *texture = EngineLoadTexture (fileName);
    else
        printf ("File '%s' doesn't exists!\n", fileName);
}

void LoadTextures()
{
    char textBuffer[150] = "";

    /* TEXTURES for PLAYER */
    for (uint16 i = 0; i < 7; i++)
    {
        sprintf (textBuffer, "./media/PNG/Players/Variable sizes/Green/alienGreen%i.png", i);
        TextureLoad (&playerTextures[i], textBuffer);
    }

    /* TEXTURES for LEVEL OBJECTS */
    TextureLoad (&levelTextures, "./media/Spritesheets/spritesheet_ground.png");

    TextureLoad (&backgroundTexture, "./media/PNG/Backgrounds/blue_grass.png");

    for (uint16 i = 0; i < 14; i++)
    {
        sprintf (textBuffer, "./media/PNG/TileObjects/tileObject%i.png", i);
        TextureLoad (&tileObjectTextures[i], textBuffer);
    }
}

void TextureArraySetColor (SDL_Texture** textures, const ubyte r, const ubyte g, const ubyte b, uint16 count)
{
    for (uint16 i = 0; i < count; i++)
    {
        SDL_SetTextureColorMod (*textures, r, g, b);
        textures++;
    }
}
