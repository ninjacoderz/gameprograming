#include "MoveComponent.h"
#include "GameMath.h"
#include "Component.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder)
, mAngularSpeed(0.0f)
, mSumOfForces(Vector2::Zero)
, mMass(10.0f)
, mVelocity(Vector2::Zero)
{
	
}


void MoveComponent::Update(float deltaTime)
{   
    if (!GameMath::NearZero(mAngularSpeed)){
        float rot = mOwner->GetRotation();
        rot += mAngularSpeed * deltaTime;
        mOwner->SetRotation(rot);
    }

    if(mSumOfForces.LengthSq() > 0 || (mVelocity.x != 0 || mVelocity.y !=0)){
        Vector2 pos = mOwner->GetPosition();
        if(mMass != 0) {
            Vector2 acc = mSumOfForces / mMass;
            mVelocity += acc * deltaTime;
        }
        pos += mVelocity * deltaTime;

        if (pos.x < 0.0f - 32.0f) { pos.x = 1024.0f +32.0f; }
        else if (pos.x > 1024.0f + 32.0f) { pos.x = 0.0f - 32.0f; }
        if (pos.y < 0.0f - 32.0f) { pos.y = 768.0f + 32.0f; }
        else if (pos.y > 768.0f + 32.0f) { pos.y = 0.0f - 32.0f; }
        mOwner->SetPosition(pos);
        mSumOfForces = Vector2::Zero;
    }
}

void MoveComponent::AddForce(Vector2 force){ 
	mSumOfForces += force;
}
