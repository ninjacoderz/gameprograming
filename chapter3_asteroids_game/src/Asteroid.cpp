#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "Game.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
{
    Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f,768.0f));
    SetPosition(randPos);
    SetRotation(Random::GetFloatRange(0.0f, GameMath::TwoPi));

    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

    MoveComponent* mc = new MoveComponent(this);
    mc->SetMass(1.0f);
    mc->SetVelocity(Vector2(GameMath::Cos(GetRotation()),GameMath::Sin(GetRotation())) * 100);

    mCircle = new CircleComponent(this);
    mCircle->SetRadius(40.0f);
    
}

Asteroid::~Asteroid()
{
    GetGame()->RemoveAsteroid(this);
}