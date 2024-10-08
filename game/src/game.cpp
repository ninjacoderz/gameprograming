#include "game.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

Game::Game()
{
}

Game::Game(SDL_Window *_window,
           SDL_Renderer *_renderer)
{ 
    this->init( _window, _renderer );
}

Game::~Game()
{
    this->_window = NULL;
    this->_renderer = NULL;
}

void Game::init( SDL_Window* _window,
        SDL_Renderer* _renderer)
{
    this->_window = _window;
    this->_renderer = _renderer;
}

void Game::gameLoop()
{
    SDL_FRect rects[16];
    const Uint64 now = SDL_GetTicks();
    int i;

    /* we'll have the rectangles grow and shrink over a few seconds. */
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);  /* black, full alpha */
    SDL_RenderClear(_renderer);  /* start with a blank canvas. */

    /* Rectangles are comprised of set of X and Y coordinates, plus width and
       height. (0, 0) is the top left of the window, and larger numbers go
       down and to the right. This isn't how geometry works, but this is
       pretty standard in 2D graphics. */

    /* Let's draw a single rectangle (square, really). */
    rects[0].x = rects[0].y = 100;
    rects[0].w = rects[0].h = 100 + (100 * scale);
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);  /* red, full alpha */
    SDL_RenderRect(_renderer, &rects[0]);

    /* Now let's draw several rectangles with one function call. */
    for (i = 0; i < 3; i++) {
        const float size = (i+1) * 50.0f;
        rects[i].w = rects[i].h = size + (size * scale);
        rects[i].x = (WINDOW_WIDTH - rects[i].w) / 2;  /* center it. */
        rects[i].y = (WINDOW_HEIGHT - rects[i].h) / 2;  /* center it. */
    }
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);  /* green, full alpha */
    SDL_RenderRects(_renderer, rects, 3);  /* draw three rectangles at once */

    /* those were rectangle _outlines_, really. You can also draw _filled_ rectangles! */
    rects[0].x = 400;
    rects[0].y = 50;
    rects[0].w = 100 + (100 * scale);
    rects[0].h = 50 + (50 * scale);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);  /* blue, full alpha */
    SDL_RenderFillRect(_renderer, &rects[0]);

    /* ...and also fill a bunch of rectangles at once... */
    for (i = 0; i < SDL_arraysize(rects); i++) {
        const float w = (float) (WINDOW_WIDTH / SDL_arraysize(rects));
        const float h = i * 8.0f;
        rects[i].x = i * w;
        rects[i].y = WINDOW_HEIGHT - h;
        rects[i].w = w;
        rects[i].h = h;
    }
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);  /* white, full alpha */
    SDL_RenderFillRects(_renderer, rects, SDL_arraysize(rects));

    SDL_RenderPresent(_renderer);  /* put it all on the screen! */
}

void Game::handleEvents(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        SDL_Log("mouse down");
        break;
    case SDL_EVENT_MOUSE_MOTION:
        SDL_Log("mouse move %f %f", event->motion.x, event->motion.y);
    case SDL_EVENT_QUIT:
        /* code */
        break;
    
    default:
        break;
    }
}