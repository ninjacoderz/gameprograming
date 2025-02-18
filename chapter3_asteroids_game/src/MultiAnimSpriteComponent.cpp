#include "MultiAnimSpriteComponent.h"
#include "GameMath.h"
#include "Actor.h"
#include <iostream>


MultiAnimSpriteComponent::MultiAnimSpriteComponent(Actor *owner, int numframesPerRow, int numRows, int drawOrder)
    :AnimSpriteComponent(owner, drawOrder)
{
    framesPerRow = numframesPerRow;
    rows = numRows;
    AnimSpriteComponent::SetAnimFPS(6.0f);
}

void MultiAnimSpriteComponent::Draw(SDL_Renderer *renderer)
{   
    SDL_FRect spriteBoundingRec;
    spriteBoundingRec.w = static_cast<int>(frameWidth * mOwner->GetScale());
    spriteBoundingRec.h = static_cast<int>(frameHeight * mOwner->GetScale());
    // Center the rectangle around the position of the owner
    spriteBoundingRec.x = static_cast<int>(mOwner->GetPosition().x - spriteBoundingRec.w / 2);
    spriteBoundingRec.y = static_cast<int>(mOwner->GetPosition().y - spriteBoundingRec.h / 2);
    
    SDL_FRect rect;
    getCurrAnimSrcRect(rect);

    SDL_RenderTextureRotated(renderer,
        mTexture,
        &rect,
        &spriteBoundingRec,
        -GameMath::ToDegrees(mOwner->GetRotation()),
        nullptr,
        SDL_FLIP_NONE);
}

void MultiAnimSpriteComponent::Update(float deltaTime)
{
    SpriteComponent::Update(deltaTime);

    if (currAnim.numFrames > 0) {

		// Update the current frame based on frame rate and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		// Wrap current frame if needed
		if (mCurrFrame >= currAnim.numFrames) {

			mCurrFrame -= currAnim.numFrames; // = 0;??
		}
	}
}

void MultiAnimSpriteComponent::SetTexture(SDL_Texture *spriteSheet)
{
    SpriteComponent::SetTexture(spriteSheet);
    frameWidth = SpriteComponent::GetTexWidth() / framesPerRow;
	frameHeight = SpriteComponent::GetTexHeight() / rows;
}

void MultiAnimSpriteComponent::setCurrentAnimation(int index)
{
    if (index != animIndex) {
		animIndex = index;
		currAnim = animations[index];
		mCurrFrame = 0.0f;
	}
}

void MultiAnimSpriteComponent::setAnimations(std::vector<Animation> anims)
{
    animations = anims;
    setDefaultAnim(0);
}

void MultiAnimSpriteComponent::getCurrAnimSrcRect(SDL_FRect &rect) {

	rect.x = frameWidth * ((currAnim.startIndex + static_cast<int>(mCurrFrame)) % framesPerRow);
	rect.y = frameHeight * ((currAnim.startIndex + static_cast<int>(mCurrFrame)) / framesPerRow);
	rect.w = frameWidth;
	rect.h = frameHeight;

	std::cout << "X: " << rect.x <<
	std::endl << "Y: " << rect.y <<
	std::endl << "W: " << rect.w <<
	std::endl << "H: " << rect.h << std::endl;

	std::cout << static_cast<int>(mCurrFrame) << std::endl;
}

void MultiAnimSpriteComponent::startIdleState() {
	currAnim = defaultAnim;
	animIndex = 0;
}
