
#pragma once
#include "Component.h"
#include <vector>
#include "GameMath.h"
#include "Actor.h"

class MoveComponent: public Component 
{
    public:
        MoveComponent(class Actor* owner, int updateOrder = 10);
        void Update(float deltaTime) override;

        float GetAnularSpeed() const { return mAngularSpeed; }
        float GetForwardSpeed() const { return mForwardSpeed; }

        void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
        void SetAngularSpeed(float spped) { mAngularSpeed = spped; }
        
    private: 
        float mAngularSpeed;
        float mForwardSpeed;
};