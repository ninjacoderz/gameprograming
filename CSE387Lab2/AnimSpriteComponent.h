// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
	public:

	AnimSpriteComponent( class Actor* owner, int drawOrder = 100 );

	// Update animation every frame (overriden from component)
	void virtual Update( float deltaTime ) override;

	// Set the textures used for animation
	void SetAnimTextures( const std::vector<SDL_Texture*>& textures );

	// Set/get the animation FPS
	float GetAnimFPS( ) const { return mAnimFPS; }
	void SetAnimFPS( float fps ) { mAnimFPS = fps; }
	
	protected:

	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;

	// Current frame displayed
	float mCurrFrame = 0.0f;

	// Animation frame rate
	float mAnimFPS = 24.0f;
};
