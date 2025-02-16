#pragma once
#include <string>
#include "Game.h"
#include "SpriteSheetActor.h"

class TileMapGame: public Game
{
    public: 
        TileMapGame(SDL_Window* window, SDL_Renderer* renderer);
        void LoadData() override;
        void UnloadData(); 
        void ProcessInput(SDL_Event *event);
        bool Initialize(SDL_Window* _window, SDL_Renderer* _renderer) override;
        
    private: 
        SpriteSheetActor* player;
        Actor* tileActor;
};