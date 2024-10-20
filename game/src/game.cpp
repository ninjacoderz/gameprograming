#include "game.h"
#include <SDL3/SDL.h>

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game():mWindow(nullptr),mRenderer(nullptr)
,mTicksCount(0)
{
    SDL_Log("Game Created");
}

bool Game::Initialize( SDL_Window* window, SDL_Renderer* renderer, SDL_GLContext& context )
{
    if(window != NULL && renderer != NULL)
    {    
        this->mWindow = window;
        this->mRenderer = renderer;
        this->mContext = context;
        return true;
    } else {
        return false;
    }
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
    SDL_GL_DestroyContext(mContext);
    SDL_Log("Game Shutdown");
	SDL_Quit();
}

void Game::RunLoop()
{
    UpdateGame();
    GenerateOutput();
}

void Game::UpdateGame()
{
    SDL_Log("Game Updated");
	
}

void Game::GenerateOutput()
{
    // Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::ProcessInput(SDL_Event *event)
{
    switch (event->type)
    {
        // If we get an SDL_QUIT event, end loop
        case SDL_EVENT_QUIT:
            // Game Quit
            break;
        case SDL_EVENT_KEY_DOWN:
            if(event->key.scancode == SDL_SCANCODE_W){
                // Handle Keycode 
            }
            break;
         case SDL_EVENT_KEY_UP:

            break;
    }

    
}