#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Texture.h"
#include "Laser.h"

Ship::Ship(Game *game)
    :Actor(game)
    ,mLaserCooldown(0.0f)
{   
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Ship.png"));

    InputComponent* ic = new InputComponent(this);
    ic->SetForwardKey(SDL_SCANCODE_W);
    ic->SetBackKey(SDL_SCANCODE_S);
    ic->SetClockwiseKey(SDL_SCANCODE_A);
    ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
    ic->SetMaxForwardSpeed(300.0f);
    ic->SetMaxAngularSpeed(GameMath::TwoPi);
}

void Ship::Update(float deltaTime)
{
    Actor::Update(deltaTime);
    mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const SDL_Scancode code)
{  
    // Need refactor because we draw and update the ship in the same function
    SDL_Log("Space Key Pressed %d", code);
    if (code == SDL_SCANCODE_SPACE && mLaserCooldown <= 0.0f)
	{
        SDL_Log("Space Key Pressed");
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}