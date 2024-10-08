#pragma once
#include <iostream>
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

enum GameState { PLAY, EXIT };

class Game {
    public: 
        Game();
        Game(SDL_Window *_window, SDL_Renderer *_renderer);
        ~Game();
        void gameLoop();
        void handleEvents(SDL_Event* event);
    private:
        void init( SDL_Window* _window, SDL_Renderer* _renderer);
        SDL_Window* _window;
        SDL_Renderer* _renderer;

        GameState _gameState;
};