#include "MoveComponent.h"
#include "GameMath.h"
#include "Component.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}


void MoveComponent::Update(float deltaTime)
{
    if (!GameMath::NearZero(mAngularSpeed)){
        float rot = mOwner->GetRotation();
        rot += mAngularSpeed * deltaTime;
        mOwner->SetRotation(rot);
    }

    if (!GameMath::NearZero(mForwardSpeed))
    {
        Vector2 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		
		mOwner->SetPosition(pos);
    }
}
