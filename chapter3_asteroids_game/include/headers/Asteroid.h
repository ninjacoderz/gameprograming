
#pragma once
#include "CircleComponent.h"
#include "Actor.h"
#include "GameMath.h"
class Asteroid: public Actor 
{
    public:
        Asteroid(class Game* game);
        ~Asteroid();
        CircleComponent* GetCircle() { return mCircle; }
    private:
        CircleComponent* mCircle;
};