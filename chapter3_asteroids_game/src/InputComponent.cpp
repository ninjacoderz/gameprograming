#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner)
:MoveComponent(owner)
{
	
}

void InputComponent::ProcessInput(const SDL_Scancode code)
{
	Vector2 force = Vector2::Zero;
    if(code == mForwardKey){
        force = Vector2(GameMath::Cos(mOwner->GetRotation()),-GameMath::Sin(mOwner->GetRotation())) * mAcceleration;
    }
    if(code == mBackKey){
        force = Vector2::Zero - Vector2(GameMath::Cos(mOwner->GetRotation()),-GameMath::Sin(mOwner->GetRotation())) * mAcceleration;
    }
    AddForce(force);

	float angularSpeed = 0.0f;
    if(code == mClockwiseKey){
        angularSpeed -= mMaxAngularSpeed;
    }
    if(code == mCounterClockwiseKey){
        angularSpeed += mMaxAngularSpeed;
    }
    SetAngularSpeed(angularSpeed);
	
}
