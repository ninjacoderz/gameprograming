#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <GL/glew.h>
#include "Game.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Texture.h"
#include "GameMath.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

Game::Game()
:mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mUpdatingActors(false){
	SDL_Log("Game no-args constructor called");
}

bool Game::Initialize()
{	
	if (!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO ) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	SDL_Log("Game initialized");
    mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	LoadData();
	mTicksCount = SDL_GetTicks();
    return true;
}

void Game::RunLoop()
{
    UpdateGame();
    GenerateOutput();
    mTicksCount = SDL_GetTicks(); 
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


void Game::Shutdown()
{
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
		delete mRenderer;
		mRenderer = nullptr;
	}

	SDL_Quit();
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

void Game::UpdateGame()
{	
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
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
	mRenderer->Draw();
}


void Game::LoadData(){

	// Create Data Here
	Actor* a = new Actor(this);
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -GameMath::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, GameMath::Pi + GameMath::Pi / 4.0f));
	a->SetRotation(q);

	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	a = new Actor(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
}