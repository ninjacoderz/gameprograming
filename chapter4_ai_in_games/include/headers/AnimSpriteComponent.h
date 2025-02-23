#pragma once
#include "SpriteComponent.h"
#include <unordered_map>
#include <vector>
using namespace std;
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
    void SetCurrAnim(int currAnim);
    void SetAnimOnce() { mAnimOnce = true; }

    float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
};