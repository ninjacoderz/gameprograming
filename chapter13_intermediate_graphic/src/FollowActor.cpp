//
// Created by Binh Nguyen Thanh on 26/05/2025.
//

#include "FollowActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "Mesh.h" 
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"

FollowActor::FollowActor(Game* game)
    :Actor(game)
    ,mMoving(false)
{
    mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/CatWarrior.gpmesh"));
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/CatWarrior.gpskel"));
	mMeshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mMoveComp = new MoveComponent(this);
    mCameraComp = new FollowCamera(this);
    mCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const InputState& code)
{
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;
    // wasd movement
    if (code.Keyboard.GetKeyValue(SDL_SCANCODE_W))
    {
        forwardSpeed += 400.0f;
    }
    if (code.Keyboard.GetKeyValue(SDL_SCANCODE_S))
    {
        forwardSpeed -= 400.0f;
    }
    if (code.Keyboard.GetKeyValue(SDL_SCANCODE_A))
    {
        angularSpeed -= GameMath::Pi;
    }
    if (code.Keyboard.GetKeyValue(SDL_SCANCODE_D))
    {
        angularSpeed += GameMath::Pi;
    }

    // Did we just start moving?
	if (!mMoving && !GameMath::NearZero(forwardSpeed))
	{
		mMoving = true;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"), 1.25f);
	}
	// Or did we just stop moving?
	else if (mMoving && GameMath::NearZero(forwardSpeed))
	{
		mMoving = false;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
	}

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);

    // Adjust horizontal distance of camera based on speed
    if (!GameMath::NearZero(forwardSpeed))
    {
        mCameraComp->SetHorzDist(500.0f);
    }
    else
    {
        mCameraComp->SetHorzDist(350.0f);
    }
}

void FollowActor::SetVisible(bool visible) const {
    mMeshComp->SetVisible(visible);
}
