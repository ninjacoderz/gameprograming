#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner, float mass)
:MoveComponent(owner, mass)
,mForwardKey(0)
,mBackKey(0)
,mClockwiseKey(0)
,mCounterClockwiseKey(0)
{
	
}

void InputComponent::ProcessInput(const SDL_Scancode code)
{
	// Calculate forward speed for MoveComponent

    float forwardSpeed = 0.0f;
    if (code == mForwardKey)
    {
        forwardSpeed += mMaxForwardSpeed;
    }
    if (code== mBackKey)
    {
        forwardSpeed -= mMaxForwardSpeed;
    }

    SetForwardSpeed(forwardSpeed);

	// Calculate angular speed for MoveComponent
	float angularSpeed = 0.0f;
	if (code == mClockwiseKey)
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (code == mCounterClockwiseKey)
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
	
}
