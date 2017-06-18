
/* STD C */
#include <stdio.h>
#include <stdlib.h>

/* LIBRARIES */
#include "SDL2/SDL.h"
#include "chipmunk/chipmunk_private.h"

/*  */
#include "defines.h"
#include "engine.h"
#include "world.h"
#include "level.h"
#include "texture.h"
#include "tileObject.h"
#include "creature.h"
#include "player.h"

extern const float PLAYER_JUMP_HEIGHT;
extern const float LIMIT_FPS;
extern float deltaTime;

SDL_Event sdlEvent;

void InitEngine()
{
    /* init sdl */
    if (EngineStart())
        printf ("Engine started.\n");
    else
        exit (ERR_SDL_NOT_INITIALIZED);

    /* create window */
    if (EngineCreateWindow (GAME_NAME,
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL))
        printf (" + Main window created.\n");
    else
        exit (ERR_SDL_WINDOW_NOT_CREATED);

    /* create renderer */
    if (EngineCreateRenderer (-1, SDL_RENDERER_ACCELERATED))
        printf (" + Renderer created.\n");
    else
        exit (ERR_SDL_RENDERER_NOT_CREATED);

    /* init audio system */
    /*if (EngineInitAudio())
        printf (" + Audio System started.\n");
    else
        exit (ERR_SDL_AUDIO_NOT_INITIALIZED);*/
}

void StopEngine()
{
    EngineClearAllInstances();
    EngineStop();
}


void Update()
{
    EngineUpdateTime();

    PlayerUpdate ();
    PlayerUpdateFrames();

    //cpSpaceReindexShapesForBody(world, player->physBody->body);
}

void Render()
{
    EngineRenderClear();

    BackgroundDraw ();
    TileObjectsUpdateAndRender (backTiles);
    //LevelUpdateAndRender ();
    TilesUpdateAndRender();
    CreaturesUpdateAndRender ();
    TileObjectsUpdateAndRender (frontTiles);

    EngineRenderPresent();
}


int main (int argc, char* argv[])
{
    InitEngine();
    LoadTextures();
    WorldCreate();
    EngineClearAllInstances();

    LevelClear ();
    LevelLoad ("./media/maps/level01.tmx");

    bool quit = false;
    while (!quit)
    {
        Update();
        Render();

        while (SDL_PollEvent(&sdlEvent))
        {
            switch (sdlEvent.type)
            {
                case SDL_QUIT :
                {
                    quit = true;
                    continue;

                    break;
                }

                case SDL_KEYDOWN :
                {
                    switch (sdlEvent.key.keysym.sym)
                    {
                        case SDLK_ESCAPE :
                        {
                            quit = true;
                            continue;

                            break;
                        }

                        /*case SDLK_s :
                        {
                            dinamycSky = !dinamycSky;
                            break;
                        }*/

                        case SDLK_b :
                        {
                            drawBB = !drawBB;
                            break;
                        }

                        case SDLK_k :
                        {
                            printf ("camera position: \n\tx : %.2f\n\ty : %.2f\n", cameraPos.x, cameraPos.y);
                            break;
                        }

                        case SDLK_r :
                        {
                            EngineClearAllInstances();
                            WorldDestroy();
                            WorldCreate();

                            LevelClear ();
                            LevelLoad ("./media/maps/level01.tmx");

                            printf ("Game restarted! \n");
                            break;
                        }

                        case SDLK_d :
                        case SDLK_RIGHT :
                        {
                            player->xDir = 1;
                            playerMoveR = true;
                            playerMoveL = false;

                            break;
                        }

                        case SDLK_a :
                        case SDLK_LEFT :
                        {
                            player->xDir = -1;
                            playerMoveR = false;
                            playerMoveL = true;

                            break;
                        }

                        case SDLK_w :
                        case SDLK_SPACE :
                        case SDLK_UP :
                        {
                            // если на земле
                            if (player->physBody->isGrounded)
                            {
                                player->physBody->body->v = cpvadd (player->physBody->body->v, cpv (0.0, PLAYER_JUMP_HEIGHT));
                                playerCountJump = 1;    // был первый прыжок
                            }
                            else
                                // в воздухе? А второй прыжок разрешён?
                                if (playerCountJump == 2)
                                {
                                    player->physBody->body->v.y = 0.0f;
                                    player->physBody->body->v = cpvadd (player->physBody->body->v, cpv (0.0, PLAYER_JUMP_HEIGHT / 1.5f));

                                    playerCountJump = 3;
                                }

                            break;
                        }
                    }

                    break;
                }

                case SDL_KEYUP :
                {
                    switch (sdlEvent.key.keysym.sym)
                    {
                        case SDLK_d :
                        case SDLK_RIGHT :
                        {
                            playerMoveR = false;

                            break;
                        }

                        case SDLK_a :
                        case SDLK_LEFT :
                        {
                            playerMoveL = false;

                            break;
                        }

                        case SDLK_w :
                        case SDLK_SPACE :
                        case SDLK_UP :
                        {
                            // если был первый прыжок, то разрешить второй
                            if (playerCountJump == 1)
                                playerCountJump = 2;

                            break;
                        }
                    }

                    break;
                }
            }
        }

        if (playerMoveR && !playerMoveL)
            player->physBody->body->v = cpvadd(player->physBody->body->v, cpv (player->accelSpeed, 0.0f));

        if (!playerMoveR && playerMoveL)
            player->physBody->body->v = cpvadd(player->physBody->body->v, cpv (-player->accelSpeed, 0.0f));

        if (!playerMoveR && !playerMoveL)
        {
            if (fabs(player->physBody->body->v.x) > (BLOCK_SIZE >> 2))
            {
                /* добавляем вектор торможения */
                cpVect braking = cpv (-player->physBody->body->v.x / (BLOCK_SIZE >> 3), 0.0f);
                player->physBody->body->v = cpvadd (player->physBody->body->v, braking);
            }
            else
                player->physBody->body->v.x = 0.0f;
        }


        /* show fps */
        char windowTitle[30] = "";
        sprintf (windowTitle, "%19s FPS - %d", GAME_NAME, fps);  // fps не более трехзначного числа!
        EngineSetWindowTitle (windowTitle);

        WorldUpdate (1.0f/60.0f);

        SDL_Delay ((int)(LIMIT_FPS - deltaTime));
    }

    WorldDestroy();
    StopEngine();

    return ERR_NONE;
}
