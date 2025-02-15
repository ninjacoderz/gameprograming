#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
    :SpriteComponent(owner, drawOrder)
    ,mCurrFrame(0.0f)
    ,mAnimFPS(24.0f)
    ,mCurrAnim(0)
{
}

void AnimSpriteComponent::SetAnimTextures(int animState, const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures.emplace(animState,textures);
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		// SetTexture(mAnimTextures[0]); Maybe for later use
	}
}


void AnimSpriteComponent::Update(float deltaTime)
{
    SpriteComponent::Update(deltaTime);

    if (mAnimTextures[mCurrAnim].size() > 0)
    {
        // Update the current frame based on frame rate
        // and delta time
        mCurrFrame += mAnimFPS * deltaTime;
        
        // Wrap current frame if needed
        while (mCurrFrame >= mAnimTextures[mCurrAnim].size())
        {
            mCurrFrame -= mAnimTextures.size();
            mAnimOnce = false;
        }

        // Set the current texture
        SetTexture(mAnimTextures[mCurrAnim][mCurrFrame]);
    }
}

void AnimSpriteComponent::SetCurrAnim(int currAnim){
    if(!mAnimOnce || (mCurrFrame >= mAnimTextures[mCurrAnim].size())){
        mCurrAnim = currAnim;
    }
}