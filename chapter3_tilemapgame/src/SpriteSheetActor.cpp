#include "SpriteSheetActor.h"
#include "MultiAnimSpriteComponent.h"
#include "Game.h"
#include <SDL3/SDL.h>
SpriteSheetActor::SpriteSheetActor(Game* game) : Actor(game), 
    verticalSpeed(0.0f),
    lateralSpeed(0.0f)
{
    MultiAnimSpriteComponent* masc = new MultiAnimSpriteComponent(this, 4, 4);
    SDL_Texture* spriteSheet = game->GetTexture("Assets/Walking.png");
    
    std::vector<Animation> anims = {
		Animation(0, 1), // Default (idle) animation. Actor faces fromt and does nothing
		Animation(0, 4), 
		Animation(4, 4),
		Animation(8, 4),
		Animation(12, 4)
	};

    masc->setAnimations(anims);
	masc->SetTexture(spriteSheet);

    animationComponent = masc;
};

void SpriteSheetActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);
    Vector2 pos = GetPosition();
	pos.x += lateralSpeed * deltaTime;
	pos.y += verticalSpeed * deltaTime;
	// Restrict position to left half of screen
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);
};

void SpriteSheetActor::ProcessKeyboard(const SDL_Scancode code) {
	
	SDL_Log("SpriteSheetActor process keyboard");
	lateralSpeed = 0.0f;
	verticalSpeed = 0.0f;

	// right/left
	if (code == SDL_SCANCODE_D) // Walk right animation on 'd' press
	{
		lateralSpeed += 250.0f;
		animationComponent->setCurrentAnimation(4);
	}
	if (code == SDL_SCANCODE_A) // walk left animation on 'a' press
	{
		lateralSpeed -= 250.0f;
		animationComponent->setCurrentAnimation(3);
	}
	// up/down
	if (code == SDL_SCANCODE_S) // walk 'down' animation on 's' press
	{
		verticalSpeed += 300.0f;
		if (lateralSpeed == 0 && animationComponent->getAnimIndex() != 1) { // dont have a diagonal animation so if it is moving up and sidewways play the sideways animation
			animationComponent->setCurrentAnimation(1);
		}
	}
	if (code == SDL_SCANCODE_W) // Walk 'up' animation on 'w' press
	{
		verticalSpeed -= 300.0f;
		if (lateralSpeed == 0 && animationComponent->getAnimIndex() != 2) { // dont have a diagonal animation so if it is moving up and sidewways play the sideways animation
			animationComponent->setCurrentAnimation(2);
		}
	}

	if (lateralSpeed == 0 && verticalSpeed == 0) { // when no key is pressed it runs the idle 'animation' (only the first frame of the sprite sheet)
		animationComponent->startIdleState();
	}
}
