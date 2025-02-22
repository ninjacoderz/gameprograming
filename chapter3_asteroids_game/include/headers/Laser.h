#pragma once
#include "Actor.h"
#include "CircleComponent.h"

class Laser : public Actor {
public:
    Laser(class Game* game,Vector2 position,float rotation);
    void UpdateActor(float deltaTime) override;
private:
    float mLifeLength;
    CircleComponent* mCircle;
};
