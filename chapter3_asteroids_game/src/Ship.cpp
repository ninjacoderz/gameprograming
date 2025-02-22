#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include "InputComponent.h"
#include "GameMath.h"
#include "CircleComponent.h"
#include "Laser.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	:Actor(game)
	,mRightSpeed(0.0f)
	,mDownSpeed(0.0f)
	,mRespawnTime(1.5f)
	,mRespawn(false)
	,isMoving(false)
{
	// Create an animated sprite component
	mAnimSprite = new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> idle = {game->GetTexture("Assets/Ship.png")};
    std::vector<SDL_Texture*> move = {game->GetTexture("Assets/ShipWithThrust.png")};
    mAnimSprite->SetAnimTextures(IDLE,idle);
    mAnimSprite->SetAnimTextures(MOVE,move);

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

	mCircle = new CircleComponent(this);
    mCircle->SetRadius(20.0f);
}

void Ship::UpdateActor(float deltaTime)
{   
	Actor::UpdateActor(deltaTime);
    if(!mRespawn) {
        for (auto ast : GetGame()->GetAsteroids()) {
            if (Intersect(*mCircle, *(ast->GetCircle()))) {
                SetScale(0.0f);
                mRespawn = true;
            }
        }
        if (isMoving) {
            mAnimSprite->SetCurrAnim(MOVE);
        } else {
            mAnimSprite->SetCurrAnim(IDLE);
        }
    }
    else{
        mRespawnTime -= deltaTime;
        if(mRespawnTime <= 0) {
            SetScale(1.0f);
            inputComp->SetVelocity(Vector2::Zero);
            SetPosition(Vector2(1024.0f / 2.0f, 768.0f / 2.0f));
            SetRotation(0.0f);
            mRespawnTime = 1.5f;
            mRespawn = false;
        }
    }
    mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const SDL_Scancode code )
{
	if(code == SDL_SCANCODE_SPACE && mLaserCooldown <= 0 && !mRespawn){
        new Laser(GetGame(),GetPosition(),GetRotation());
        mLaserCooldown = 0.5f;
    }
    if(code == SDL_SCANCODE_W || code == SDL_SCANCODE_S  || code == SDL_SCANCODE_A || code == SDL_SCANCODE_D){
        isMoving = true;
    }
    else{
        isMoving = false;
    }
}
