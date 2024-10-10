#include "game.h"
#include <SDL3/SDL.h>

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game():mWindow(nullptr),mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDir(0)
,numBalls(5)
{
    SDL_Log("Game Created");
}

bool Game::Initialize( SDL_Window* window, SDL_Renderer* renderer )
{
    if(window != NULL && renderer != NULL)
    {    
        this->mWindow = window;
        this->mRenderer = renderer;
        
        // Init for some other game variables
        
        mPaddlePos.x = 10.0f;
        mPaddlePos.y = 768.0f/2.0f;
        mPaddlePosSecond.x = 1004.0f;
        mPaddlePosSecond.y = 768.0f/2.0f;
    
        
        for (int i =0; i < numBalls; i++) {
            Vector2 mBallPos;
            Vector2 mBallVel;
            mBallPos.x = 1024.0f/2.0f + i * 50;
            mBallPos.y = 768.0f/2.0f;
            mBallVel.x = i%2 == 0 ? 200.0f : -200.0f;
            mBallVel.y = 235.0f;
            Ball ball = {
                mBallPos, mBallVel
            };
            balls.push_back(ball);
        }
        

        return true;
    } else {
        return false;
    }
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
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

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH/2.0f + thickness))
		{
			mPaddlePos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}
	
    if (mPaddleDirSecond != 0)
	{
		mPaddlePosSecond.y += mPaddleDirSecond * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePosSecond.y < (paddleH/2.0f + thickness))
		{
			mPaddlePosSecond.y = paddleH/2.0f + thickness;
		}
		else if (mPaddlePosSecond.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePosSecond.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
    for (int i = 0; i < numBalls; i++) {
        Ball *ball = &balls[i];
        ball->mBallPos.x += ball->mBallVel.x * deltaTime;
        ball->mBallPos.y += ball->mBallVel.y * deltaTime;

        // Bounce if needed
        // Did we intersect with the paddle?
        float diff = mPaddlePos.y - ball->mBallPos.y;
        // Take absolute value of difference
        diff = (diff > 0.0f) ? diff : -diff;
        float diffSecond = mPaddlePosSecond.y - ball->mBallPos.y;
        diffSecond = (diffSecond > 0.0f) ? diffSecond : -diffSecond;
        if (
            // Our y-difference is small enough
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            ball->mBallPos.x <= 25.0f && ball->mBallPos.x >= 20.0f &&
            // The ball is moving to the left
            ball->mBallVel.x < 0.0f)
        {
            ball->mBallVel.x *= -1.0f;
        }
        // Did the ball go off the screen? (if so, end game)
        else if (ball->mBallPos.x <= 0.0f)
        {
            mIsRunning = false;
        }

        if (
            // Our y-difference is small enough
            diffSecond <= paddleH / 2.0f &&
            // We are in the correct x-position
            ball->mBallPos.x <= 1024.0f && ball->mBallPos.x >= 1004.0f &&
            // The ball is moving to the left
            ball->mBallVel.x > 0.0f)
        {
            ball->mBallVel.x *= -1.0f;
        }
        // Did the ball go off the screen? (if so, end game)
        else if (ball->mBallPos.x <= 0.0f)
        {
            mIsRunning = false;
        }
        // Did the ball collide with the right wall?
        // else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
        // {
        // 	mBallVel.x *= -1.0f;
        // }
        
        // Did the ball collide with the top wall?
        if (ball->mBallPos.y <= thickness && ball->mBallVel.y < 0.0f)
        {
            ball->mBallVel.y *= -1;
        }
        // Did the ball collide with the bottom wall?
        else if (ball->mBallPos.y >= (768 - thickness) &&
            ball->mBallVel.y > 0.0f)
        {
            ball->mBallVel.y *= -1;
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

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_FRect wall = {
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw right wall
	// wall.x = 1024 - thickness;
	// wall.y = 0;
	// wall.w = thickness;
	// wall.h = 1024;
	// SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw paddle
	SDL_FRect paddle = {
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);
    
    // Draw paddle second
	SDL_FRect paddleSecond = {
		static_cast<int>(mPaddlePosSecond.x),
		static_cast<int>(mPaddlePosSecond.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddleSecond);
	// Draw ball
    for (int i =0; i < numBalls; i++) {
        Ball ball = balls[i];
        SDL_FRect ballFRect = {	
            static_cast<int>(ball.mBallPos.x - thickness/2),
            static_cast<int>(ball.mBallPos.y - thickness/2),
            thickness,
            thickness
        };
	    SDL_RenderFillRect(mRenderer, &ballFRect);
    }
	
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::ProcessInput(SDL_Event *event)
{
    switch (event->type)
    {
        // If we get an SDL_QUIT event, end loop
        case SDL_EVENT_QUIT:
            mIsRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if(event->key.scancode == SDL_SCANCODE_W){
                mPaddleDir -= 1;
            }
            if(event->key.scancode == SDL_SCANCODE_S){
                mPaddleDir += 1;
            }

            if(event->key.scancode == SDL_SCANCODE_I){
                mPaddleDirSecond -= 1;
            }
            if(event->key.scancode == SDL_SCANCODE_K){
                mPaddleDirSecond += 1;
            }

            break;
         case SDL_EVENT_KEY_UP:
            mPaddleDir = 0;
            mPaddleDirSecond = 0;
            break;
    }

    
}