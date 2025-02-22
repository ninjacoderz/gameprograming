#include "Laser.h"

#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include "Actor.h"

Laser::Laser(Game *game,Vector2 position, float rotation)
        : Actor(game){
    SetPosition(position);
    SetRotation(rotation);
    MoveComponent* mc = new MoveComponent(this);
    mc->SetVelocity(Vector2(GameMath::Cos(rotation),-GameMath::Sin(rotation))*600.0f);
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(GetGame()->GetTexture("Assets/Laser.png"));
    mLifeLength = 1.0f;
    mCircle = new CircleComponent(this);
    mCircle->SetRadius(5.0f);
}

void Laser::UpdateActor(float deltaTime) {
    mLifeLength -= deltaTime;
    if(mLifeLength <= 0){
        SetState(EDead);
    }
    else {
        for (auto ast : GetGame()->GetAsteroids()) {
            if (Intersect(*mCircle, *(ast->GetCircle()))) {
                SetState(EDead);
                ast->SetState(EDead);
                break;
            }
        }
    }
}
