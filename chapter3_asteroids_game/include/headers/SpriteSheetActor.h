#pragma once
#include "Actor.h"
#include "MultiAnimSpriteComponent.h"

class SpriteSheetActor: public Actor
{
    public: 
        SpriteSheetActor(class Game* game);
        void UpdateActor(float deltaTime) override;
        void ProcessKeyboard ( const SDL_Scancode code);
        float getVerticalSpeed() const { return verticalSpeed; };
        float getLateralSpeed() const { return lateralSpeed; };

    private: 
        float verticalSpeed;
        float lateralSpeed;
        MultiAnimSpriteComponent* animationComponent;
};