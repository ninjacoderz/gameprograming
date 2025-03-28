
#pragma once
#include "Actor.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);

	void Update(float deltaTime) override;
	void ActorInput(const SDL_Scancode code) override;
private:
	float mLaserCooldown;
};