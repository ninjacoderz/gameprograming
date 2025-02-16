#pragma once
#include "AnimSpriteComponent.h"
#include "Animation.h"

class MultiAnimSpriteComponent : public AnimSpriteComponent 
{
    public: 
        MultiAnimSpriteComponent(Actor* owner, int numframesPerRow, int numRows, int drawOrder = 100);
        void Draw(SDL_Renderer* renderer) override;
        void Update(float deltaTime) override;
        void SetTexture(SDL_Texture* spriteSheet) override;
        void setCurrentAnimation(int index);
        void setAnimations(std::vector<Animation> anims);
        void setDefaultAnim(int index) { defaultAnim  = animations[index]; };
        void startIdleState();
        int getAnimIndex() { return animIndex; }
    private: 

        void getCurrAnimSrcRect(SDL_FRect &rect);
        int rows;
        int framesPerRow;
        int frameHeight;
        int frameWidth;
        std::vector<Animation> animations;
        Animation defaultAnim;
        Animation currAnim;
        int animIndex;
};