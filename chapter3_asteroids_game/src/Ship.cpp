#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include "InputComponent.h"
#include "GameMath.h"

Ship::Ship(Game* game)
	:Actor(game)
	,mRightSpeed(0.0f)
	,mDownSpeed(0.0f)
{
	// Create an animated sprite component
	SpriteComponent* spriteComponent = new SpriteComponent(this, 150);
	// std::vector<SDL_Texture*> anims = {
	// 	game->GetTexture("Assets/Ship.png"),
		// game->GetTexture("Assets/Ship01.png"),
		// game->GetTexture("Assets/Ship02.png"),
		// game->GetTexture("Assets/Ship03.png"),
		// game->GetTexture("Assets/Ship04.png"),
	// };
	spriteComponent->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	inputComp = new InputComponent(this);
	inputComp->SetAngularSpeed(0.0f);
	inputComp->SetAcceleration(3000.0f);
	inputComp->SetMass(1.0f);
	inputComp->SetMaxAngularSpeed(GameMath::TwoPi);

	inputComp->SetForwardKey(SDL_SCANCODE_W);
	inputComp->SetBackKey(SDL_SCANCODE_S);
	inputComp->SetClockwiseKey(SDL_SCANCODE_A);
	inputComp->SetCounterClockwiseKey(SDL_SCANCODE_D);
}

void Ship::UpdateActor(float deltaTime)
{   
	// Actor::UpdateActor(deltaTime);
	// // Update position based on speeds and delta time
	// Vector2 pos = GetPosition();
	// pos.x += mRightSpeed * deltaTime;
	// pos.y += mDownSpeed * deltaTime;
	// // Restrict position to left half of screen
	// if (pos.x < 25.0f)
	// {
	// 	pos.x = 25.0f;
	// }
	// else if (pos.x > 500.0f)
	// {
	// 	pos.x = 500.0f;
	// }
	// if (pos.y < 25.0f)
	// {
	// 	pos.y = 25.0f;
	// }
	// else if (pos.y > 743.0f)
	// {
	// 	pos.y = 743.0f;
	// }
	// SetPosition(pos);
}

void Ship::ProcessKeyboard(const SDL_Scancode code )
{
	// mRightSpeed = 0.0f;
	// mDownSpeed = 0.0f;
	// // right/left
	// if (code == SDL_SCANCODE_D)
	// {
	// 	mRightSpeed += 250.0f;
	// }
	// if (code == SDL_SCANCODE_A)
	// {
	// 	mRightSpeed -= 250.0f;
	// }
	// // up/down
	// if (code == SDL_SCANCODE_S)
	// {
	// 	mDownSpeed += 300.0f;
	// }
	// if (code == SDL_SCANCODE_W)
	// {
	// 	mDownSpeed -= 300.0f;
	// }
}
