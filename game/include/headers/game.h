#pragma once
#include <iostream>
#include <SDL3/SDL.h>

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
    Vector2 mBallVel;
    Vector2 mBallPos;
};

class Game {
    public: 
        Game();
        // Initialize the game
	    bool Initialize(SDL_Window* _window, SDL_Renderer* _renderer);
        // Shutdown the game
	    void Shutdown();
        void RunLoop();
        void ProcessInput(SDL_Event *event);

    private:
        void UpdateGame();
        void GenerateOutput();
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        // Other variable here 
        Uint64 mTicksCount;
        bool mIsRunning;
        int mPaddleDir;
        int mPaddleDirSecond;
        int numBalls;
        std::vector<Ball> balls;
        Vector2 mPaddlePos;
        Vector2 mPaddlePosSecond;
};