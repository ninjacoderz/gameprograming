#pragma once
#include "Actor.h"
#include <SDL3/SDL.h>
#include "InputComponent.h"
#include "CircleComponent.h"
#include "AnimSpriteComponent.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime);
	void ActorInput(const SDL_Scancode state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
private:
	enum AnimState{
		IDLE,
		MOVE
	};
	AnimSpriteComponent* mAnimSprite;
	float mRightSpeed;
	float mDownSpeed;
	InputComponent* inputComp;
	CircleComponent* mCircle;
	float mLaserCooldown;
    float mRespawnTime;
    bool mRespawn;
	bool isMoving;
};