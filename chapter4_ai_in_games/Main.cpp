/*
 * This example creates an SDL window and renderer, and then draws some
 * rectangles to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "Game.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
// Global game varriables
Game *game ;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }    
    
    window = SDL_CreateWindow("My First SDL Window", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_MAXIMIZED);    
    if (!window) {
        SDL_Log("SDL_CreateWindow() failed: %s\n", SDL_GetError());
    }

    if (!renderer) {
        renderer = SDL_CreateRenderer(window, NULL);
    }
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
    }

    game = new Game(window, renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{   
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    game->ProcessInput(event);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const int FPS = 60;
    const int frameTime = 1000/FPS;

    Uint32 timeStart = SDL_GetTicks();
    game->RunLoop();
    Uint32 timeEnd = SDL_GetTicks();
    int workingTime = timeEnd - timeStart;
    if(workingTime < frameTime) SDL_Delay(frameTime - workingTime);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    game->Shutdown();
    delete game;
    /* SDL will clean up the window/renderer for us. */
}