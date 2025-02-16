#pragma once
#include "SpriteComponent.h"

#include <vector>
using namespace std;
class AnimSpriteComponent : public SpriteComponent
{
protected:
    vector<SDL_Texture*> mAnimTextures;
    float mCurrFrame;
    float mAnimFPS;
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    void Update(float deltaTime) override;
    void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
    float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
};