#pragma once

#include <cstdint>
#include "Actor.h"
#include "AnimSpriteComponent.h"

class Player : public Actor {
    public: 
        Player(Game* game );
        void UpdateActor(float deltaTime) override;
        void ProcessKeyboard(const SDL_Scancode state);
        float GetRightSpeed() const { return mRightSpeed; }
	    float GetDownSpeed() const { return mDownSpeed; }
    
    private:
        enum animState{
            IDLE,
            WALK,
            JUMP,
            PUNCH
        };

        float mRightSpeed;
        float mDownSpeed;
        AnimSpriteComponent* mAnim;
        animState mCurrAnimState;
};