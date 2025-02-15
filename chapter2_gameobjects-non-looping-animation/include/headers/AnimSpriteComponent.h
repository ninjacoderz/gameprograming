#pragma once
#include "SpriteComponent.h"
#include <unordered_map>
#include <vector>
class AnimSpriteComponent : public SpriteComponent
{
protected:
    std::unordered_map<int,std::vector<SDL_Texture*>> mAnimTextures;
    float mCurrFrame;
    float mAnimFPS;
    int mCurrAnim;
    bool mAnimOnce;

public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    void Update(float deltaTime) override;

    void SetAnimTextures(int animState, const std::vector<SDL_Texture*>& textures);
    float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
    void SetAnimOnce() { mAnimOnce = true; }
    void SetCurrAnim (int currAnim);
};