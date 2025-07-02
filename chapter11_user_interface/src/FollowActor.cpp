//
// Created by Binh Nguyen Thanh on 26/05/2025.
//

#include "FollowActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"

FollowActor::FollowActor(Game* game)
    :Actor(game)
{
    mMeshComp = new MeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
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
