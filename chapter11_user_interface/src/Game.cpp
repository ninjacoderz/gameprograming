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
#include "PlaneActor.h"
#include "SpriteComponent.h"
#include "AudioSystem.h"
#include "SoundEvent.h"
#include "AudioComponent.h"
#include "FollowActor.h"
#include "InputSystem.h"
#include "FPSActor.h"
#include "OrbitActor.h"
#include "PhysWorld.h"
#include "SplineActor.h"
#include "TargetActor.h"
#include "SDL_ttf.h"
#include "Font.h"
#include "UIScreen.h"
#include "HUD.h"
#include "PauseMenu.h"
#include <fstream>
#include <sstream>
#include "rapidjson/document.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

Game::Game()
:mRenderer(nullptr)
,mTicksCount(0)
,mPhysWorld(nullptr)
,mGameState(EGameplay)
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

	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	mInputSystem = new InputSystem();
	if(!mInputSystem->Initialize())
	{
		SDL_Log("Failed to initialize input system");
		return false;
	}
	mInputSystem->SetRelativeMouseMode(mRenderer->getWindow(), true);

	// Create the physics world
	mPhysWorld = new PhysWorld(this);

	// Initilize SDL_tff
	if(TTF_Init() == 0) {
		SDL_Log("Failed to initilize SDL_ttf");
		return false;
	}

	LoadData();
	mTicksCount = SDL_GetTicks();
    return true;
}

void Game::RunLoop()
{	
	while (mGameState != EQuit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			mGameState = EQuit;
			break;
		
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (mGameState == EGameplay)
			{
				HandleKeyPress(event.button.button);
			}
			else if (!mUIStack.empty())
			{
				mUIStack.back()->
					HandleKeyPress(event.button.button);
			}
			break;

		case SDL_EVENT_KEY_DOWN:
			if (!event.key.repeat)
			{
				if (mGameState == EGameplay)
				{
					SDL_Log("Handle Press Key In Game");
					HandleKeyPress(event.key.key);
				}
				else if (!mUIStack.empty())
				{
					mUIStack.back()->
						HandleKeyPress(event.key.key);
				}
			}
			break;
			
		default:
			break;
		}
		
	}

	mInputSystem->PrepareForUpdate();

	if (mGameState != EGameplay) {
		mInputSystem->SetRelativeMouseMode(mRenderer->getWindow(), false);
	}
	else {
		mInputSystem->SetRelativeMouseMode(mRenderer->getWindow(), true);
	}

	mInputSystem->Update();

	const InputState& state = mInputSystem->GetState();
	
	if (mGameState == EGameplay)
	{
		mUpdatingActors = true;

		for (auto actor : mActors)
		{
			actor->ProcessInput(state);
		}
		mUpdatingActors = false;
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->ProcessInput(state);
	}

}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
	{
		// Create pause menu
		SDL_Log("Pause Menu");
		new PauseMenu(this);
		break;
	}
	case '-':
	{
		// Reduce master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = GameMath::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		// Increase master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = GameMath::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	} 
	case '1':
	{
		// Load English text
		LoadText("Assets/English.gptext");
		break;
	}
	case '2':
	{
		// Load Russian text
		LoadText("Assets/Russian.gptext");
		break;
	}
	case SDL_BUTTON_LEFT:
	{
		// Fire weapon
		mFPSActor->Shoot();
		break;
	}
	default:
		break;
	}
}


void Game::Shutdown()
{
	UnloadData();
	TTF_Quit();
	delete mPhysWorld;
	if (mRenderer)
	{
		mRenderer->Shutdown();
		delete mRenderer;
		mRenderer = nullptr;
	}
	if(mInputSystem)
	{
		mInputSystem->Shutdown();
		delete mInputSystem;
		mInputSystem = nullptr;
	}
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
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
	mTicksCount = SDL_GetTicks(); 
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
	mAudioSystem->Update(deltaTime);

	// Update UI
	for(auto ui : mUIStack){
		if(ui->GetState() == UIScreen::EActive) {
			ui->Update(deltaTime);
		}
	}

	// Delete any UIScreens that are closed
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete *iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}


void Game::LoadData(){

	// Load English text
	LoadText("Assets/English.gptext");

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
	AudioComponent* ac = new AudioComponent(a);
	ac->PlayEvent("event:/FireLoop");

	// Start music
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

	// Setup floor
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// Left/right walls
	q = Quaternion(Vector3::UnitX, GameMath::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, GameMath::PiOver2));
	// Forward/back walls
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// a = new Actor(this);
	// a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	// SpriteComponent* sc = new SpriteComponent(a);
	// sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));

	// a = new Actor(this);
	// a->SetPosition(Vector3(-390.0f, 275.0f, 0.0f));
	// sc = new SpriteComponent(a);
	// sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));

	// a = new Actor(this);
	// a->SetScale(2.0f);
	// mCrosshair = new SpriteComponent(a);
	// mCrosshair->SetTexture(mRenderer->GetTexture("Assets/Crosshair.png")); 

	// Different camera actors
	mFPSActor = new FPSActor(this);
	mFollowActor = new FollowActor(this);
	mOrbitActor = new OrbitActor(this);
	mSplineActor = new SplineActor(this);

	// UI elements
	mHUD = new HUD(this);

	// Create target actors
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));

	

	ChangeCamera(1);
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

void Game::ChangeCamera(int mode) const {
	// Disable everything
	mFPSActor->SetState(Actor::EPaused);
	mFPSActor->SetVisible(false);
	// mCrosshair->SetVisible(false);
	mFollowActor->SetState(Actor::EPaused);
	mFollowActor->SetVisible(false);

	mOrbitActor->SetState(Actor::EPaused);
	mOrbitActor->SetVisible(false);

	mSplineActor->SetState(Actor::EPaused);
	
	// Enable the camera specified by the mode
	switch (mode)
	{
		case 1:
			mFPSActor->SetState(Actor::EActive);
			mFPSActor->SetVisible(true);
			// mCrosshair->SetVisible(true);
			break;
		case 2:
			mFollowActor->SetState(Actor::EActive);
			mFollowActor->SetVisible(true);
			break;

		case 3:
			mOrbitActor->SetState(Actor::EActive);
			mOrbitActor->SetVisible(true);
			break;
		case '4':
			mSplineActor->SetState(Actor::EActive);
			mSplineActor->RestartSpline();
			break;
		default:
			break;
	}
}

void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}

const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	// Find this text in the map, if it exists
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}

Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}

void Game::LoadText(const std::string& fileName)
{
	// Clear the existing map, if already loaded
	mText.clear();
	// Try to open the file
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}
	// Read the entire file to a string stream
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// Open this file in rapidJSON
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}
	// Parse the text map
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(), 
				itr->value.GetString());
		}
	}
}
