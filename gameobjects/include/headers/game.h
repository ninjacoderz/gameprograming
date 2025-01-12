#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>

class Game {
    public: 
        Game(SDL_Window* window, SDL_Renderer* renderer);
        // Initialize the game
	    bool Initialize(SDL_Window* _window, SDL_Renderer* _renderer);
        // Shutdown the game
	    void Shutdown();
        void RunLoop();
        void ProcessInput(SDL_Event *event);
        void AddActor(class Actor* actor);
        void RemoveActor(class Actor* actor);

    private:
        void UpdateGame(float detalTime);
        void GenerateOutput();
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        SDL_GLContext mContext;
        Uint32 mTicksCount;
        
        bool mUpdatingActors;
        std::vector<class Actor*> mPendingActors;
        std::vector<class Actor*> mActors;
};