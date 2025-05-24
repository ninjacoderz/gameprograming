// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"
#include "GameMath.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	if (!GameMath::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		Quaternion inc(Vector3::UnitZ, angle);
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	
	if (!GameMath::NearZero(mForwardSpeed) || !GameMath::NearZero(mStrafeSpeed))
	{
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
