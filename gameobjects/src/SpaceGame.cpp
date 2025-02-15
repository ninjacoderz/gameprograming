
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "SpaceGame.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "BGSpriteComponent.h"
#include "Game.h"

SpaceGame::SpaceGame(SDL_Window* window, SDL_Renderer* renderer)
{
    this->Initialize(window, renderer);
}

void SpaceGame::ProcessInput(SDL_Event *event) {
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
			player->ProcessKeyboard(event->key.scancode);
            break;
         case SDL_EVENT_KEY_UP:
            break;
    }
}

bool SpaceGame::Initialize(SDL_Window *_window, SDL_Renderer *_renderer)
{
    this->mWindow = _window;
    this->mRenderer = _renderer;
	
	this->LoadData();
	
    return true;
}

void SpaceGame::LoadData()
{
    SDL_Log("SpaceGame Loaded");
	player = new SpriteSheetActor(this);
	player->SetPosition(Vector2(100.0f, 384.0f));
	player->SetScale(1.5f);
}
