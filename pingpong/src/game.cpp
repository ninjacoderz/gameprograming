#include "../include/headers/game.h"
#include <SDL3/SDL.h>

const int thickness = 15;
const int paddleH = 100;
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

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

    mTicksCount = SDL_GetTicks();
    // Initilize the private variables
    mBallPos.x = WINDOW_WIDTH / 2;
    mBallPos.y = WINDOW_HEIGHT / 2;

    mPaddPos.x = 10.0f;
    mPaddPos.y = WINDOW_HEIGHT / 2;

    mPaddDir = 0;

    mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
    
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

    SDL_Log("Delta Time: %f", deltaTime);

    UpdateGame(deltaTime);
    GenerateOutput();

    mTicksCount = SDL_GetTicks();    
}

void Game::UpdateGame(float detalTime)
{
    mPaddPos.y += mPaddDir * 500 * detalTime;
    // Make sure paddle stays in bounds
	if(mPaddPos.y < paddleH/2 + thickness)
    {
        mPaddPos.y = paddleH/2 + thickness;
    }
    else if(mPaddPos.y > WINDOW_HEIGHT - paddleH/2 - thickness)
    {
        mPaddPos.y = WINDOW_HEIGHT - paddleH/2 - thickness;
    }

    // Update ball position
    mBallPos.x += mBallVel.x * detalTime;
    mBallPos.y += mBallVel.y * detalTime;
    
    if(mBallPos.y <= thickness && mBallVel.y < 0.0f)
    {
        mBallVel.y *= -1;
    }
    else if(mBallPos.y >= WINDOW_HEIGHT - thickness && mBallVel.y > 0.0f)
    {
        mBallVel.y *= -1;
    }

    if(mBallPos.x <= thickness && mBallVel.x < 0.0f)
    {
        if(mBallPos.y > mPaddPos.y - paddleH/2 && mBallPos.y < mPaddPos.y + paddleH/2)
        {
            mBallVel.x *= -1;
        }
        else
        {
            // Game over
            mBallPos.x = WINDOW_WIDTH / 2;
            mBallPos.y = WINDOW_HEIGHT / 2;
            mBallVel.x = 200.0f;
            mBallVel.y = 235.0f;
        }
    }
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

    // Draw the walls
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_FRect wall = {0, 0, WINDOW_WIDTH, thickness};
    SDL_RenderFillRect(mRenderer, &wall);

    SDL_FRect ball = {
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    SDL_RenderFillRect(mRenderer, &ball);
    
    SDL_FRect paddle = {
        static_cast<int>(mPaddPos.x - thickness/2) + thickness,
        static_cast<int>(mPaddPos.y - paddleH/2),
        thickness,
        paddleH
    };
    SDL_RenderFillRect(mRenderer, &paddle);

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
                mPaddDir -= 1;
                SDL_Log("W Key Pressed");
            }
            if(event->key.scancode == SDL_SCANCODE_S){
                // Handle Keycode 
                mPaddDir += 1;
                SDL_Log("S Key Pressed");
            }
            break;
         case SDL_EVENT_KEY_UP:
            mPaddDir = 0;
            break;
    }

    
}