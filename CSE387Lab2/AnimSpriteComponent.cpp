// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "MathLibsConstants.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	// Call the super class update
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {

		// Update the current frame based on frame rate and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		// Wrap current frame if needed
		if (mCurrFrame >= mAnimTextures.size()) {

			mCurrFrame -= mAnimTextures.size(); // = 0;??
		}

		// Set the current texture
		SpriteComponent::SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;

	if (mAnimTextures.size() > 0) {
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SpriteComponent::SetTexture(mAnimTextures[0]);
	}
}
