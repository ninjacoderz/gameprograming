#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "Game.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
    {
        // Initialize to random position/orientation
        Vector2 randPos = Random::GetVector(Vector2::Zero,
            Vector2(1024.0f, 768.0f));
        SetPosition(randPos);
    
        SetRotation(Random::GetFloatRange(0.0f, GameMath::TwoPi));
    
        // Create a sprite component
        SpriteComponent* sc = new SpriteComponent(this);
        sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
    
        // Create a move component, and set a forward speed
        MoveComponent* mc = new MoveComponent(this);
        mc->SetForwardSpeed(150.0f);
    }

Asteroid::~Asteroid()
{
    GetGame()->RemoveAsteroid(this);
}

void Asteroid::SetPosition(const Vector2 &pos)
{
    {
        // (Screen wrapping code only for asteroids)
        mPosition = pos;

        if (mPosition.x < 0.0f) { mPosition.x = 1022.0f; }
        else if (mPosition.x > 1024.0f) { mPosition.x = 2.0f; }

        if (mPosition.y < 0.0f) { mPosition.y = 766.0f; }
        else if (mPosition.y > 768.0f) { mPosition.y = 2.0f; }
        
    }
}
