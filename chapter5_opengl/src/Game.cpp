#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <GL/glew.h>
#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "Asteroid.h"
#include "Texture.h"
#include "GameMath.h"

Game::Game(){
	SDL_Log("Game no arg constructor called");
}

Game::Game(SDL_Window *window, SDL_GLContext context)
{ 
	SDL_Log("Game constructor called");
    this->Initialize( window , context);
}

bool Game::Initialize( SDL_Window* window, SDL_GLContext context)
{
    this->mWindow = window;
	this->mContext = context;
    
    LoadShaders();
	InitSpriteVerts();
    LoadData();

    return true;
}

void Game::Shutdown()
{
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;

	SDL_DestroyWindow(mWindow);
	SDL_GL_DestroyContext(mContext);
    SDL_Log("Game Shutdown");
	SDL_Quit();
}

void Game::RunLoop()
{
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    UpdateGame(deltaTime);
    GenerateOutput();

    mTicksCount = SDL_GetTicks(); 
}

void Game::InitSpriteVerts(){

    float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
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
        pending->ComputeWorldTransform();
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
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Draw all sprite components
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

    for (SpriteComponent* sprite : mSprites)
    {
        sprite->Draw(mSpriteShader);
    }

	SDL_GL_SwapWindow(mWindow);
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
                SDL_Log("W Key Pressed %d", event->key.scancode);
            }
			SDL_Log("W Key Pressed %d", event->key.scancode);
			// Call Actor Input 
			mUpdatingActors = true;
			for (auto actor : mActors)
			{
				actor->ProcessInput(event->key.scancode);
			}
			mUpdatingActors = false;
            break;
         case SDL_EVENT_KEY_UP:
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

void Game::LoadData(){

	// Create player's ship
	Ship* ship = new Ship(this);
	ship->SetPosition(Vector2(0.0f, 0.0f));

	// Create asteroids
	const int numAsteroids = 10;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
	
}

void Game::AddSprite(class SpriteComponent* sprite)
{
    // Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
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

void Game::RemoveSprite(class SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

bool Game::LoadShaders()
{
    mSpriteShader = new Shader();
    if(!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
        return false;
    }

	mSpriteShader->SetActive();

    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.0f, 768.0f);

    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
    
    return true;
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
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

}
void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}