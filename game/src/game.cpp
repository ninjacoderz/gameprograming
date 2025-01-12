#include "game.h"
#include <SDL3/SDL.h>

Game::Game(SDL_Window *_window,
           SDL_Renderer *_renderer)
{ 
    this->Initialize( _window, _renderer );
}

bool Game::Initialize( SDL_Window* _window,
        SDL_Renderer* _renderer)
{
    this->mWindow = _window;
    this->mRenderer = _renderer;

    if (!this->mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

    if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
    return true;
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
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    UpdateGame(deltaTime);
    GenerateOutput();

    mTicksCount = SDL_GetTicks(); 
}

void Game::UpdateGame(float detalTime)
{
    // SDL_Log("Game Updated");
	
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
                SDL_Log("W Key Pressed");
            }
            break;
         case SDL_EVENT_KEY_UP:
        
            break;
    }

    
}