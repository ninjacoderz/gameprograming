#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include "Ship.h"
#include "Enemy.h"
#include "Grid.h"

Game::Game(){
	SDL_Log("Game no arg constructor called");
}

Game::Game(SDL_Window *_window,
           SDL_Renderer *_renderer)
{ 
	SDL_Log("Game constructor called");
    this->Initialize( _window, _renderer );
}

bool Game::Initialize( SDL_Window* _window,
        SDL_Renderer* _renderer)
{
    this->mWindow = _window;
    this->mRenderer = _renderer;
	
	this->LoadData();
	
	mTicksCount = SDL_GetTicks();
    return true;
}

void Game::Shutdown()
{
	UnloadData();
    SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
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

void Game::UpdateGame(float deltaTime)
{
    mUpdatingActors = true;

    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }

    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }
	
    for (auto actor : deadActors)
    {
        delete actor;
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

	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

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
			// Actor Input...
			if (event->key.scancode == SDL_SCANCODE_B)
			{
				SDL_Log("B Key Pressed");
				mGrid->BuildTower();
			}
            break;
		case SDL_EVENT_KEY_UP:
            break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			float x, y;
			Uint32 buttons = SDL_GetMouseState(&x, &y);
			if (SDL_BUTTON_MASK(buttons) & SDL_BUTTON_LEFT)
			{
				mGrid->ProcessClick(x, y);
			}
			break;
    }

    
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor( Actor* actor )
{
    // Is it in pending actors?
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    // Is it in actors?
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	SDL_Log("Texture Name: %s", fileName.c_str());
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{

		// Create texture from file
		tex = IMG_LoadTexture(mRenderer, fileName.c_str());

		if (!tex)
		{
			SDL_Log("Failed to load image file to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::LoadData(){
	SDL_Log("Game Load");
	mGrid = new Grid(this);

	// Enemy* enemy = new Enemy(this);

	// enemy->SetPosition(Vector2(100, 100));
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for ( ;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

Enemy* Game::GetNearestEnemy(const Vector2& pos)
{
	Enemy* best = nullptr;
	
	if (mEnemies.size() > 0)
	{
		best = mEnemies[0];
		// Save the distance squared of first enemy, and test if others are closer
		float bestDistSq = (pos - mEnemies[0]->GetPosition()).LengthSq();
		for (size_t i = 1; i < mEnemies.size(); i++)
		{
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq)
			{
				bestDistSq = newDistSq;
				best = mEnemies[i];
			}
		}
	}
	
	return best;
}
