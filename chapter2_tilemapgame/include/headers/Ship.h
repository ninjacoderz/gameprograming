#pragma once
#include "Actor.h"
#include <SDL3/SDL.h>
class Ship : public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime);
	void ProcessKeyboard(const SDL_Scancode state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
private:
	float mRightSpeed;
	float mDownSpeed;
};