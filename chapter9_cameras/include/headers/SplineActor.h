#pragma once
#include "Actor.h"

class SplineActor : public Actor
{
public:
	SplineActor(class Game* game);

	void ActorInput(const InputState &keyState) override;

	void RestartSpline();
private:
	class SplineCamera* mCameraComp;
};
