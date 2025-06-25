//
// Created by Binh Nguyen Thanh on 24/05/2025.
//

#include "FPSActor.h"

#include "AudioComponent.h"
#include "MoveComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include  "Game.h"
#include "Renderer.h"

FPSActor::FPSActor(Game* game)
    :Actor(game)
{
    mMoveComp = new MoveComponent(this);
    mAudioComp = new AudioComponent(this);
    mLastFootstep = 0.0f;
    mFootstep = mAudioComp->PlayEvent("event:/Footstep");
    mFootstep.SetPaused(true);

    mCameraComp = new FPSCamera(this);

    mFPSModel = new Actor(game);
    mFPSModel->SetScale(0.75f);
    mMeshComp = new MeshComponent(mFPSModel);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));
}

void FPSActor::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);

    // Play the footstep if we're moving and haven't recently
    mLastFootstep -= deltaTime;
    if (!GameMath::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f)
    {
        mFootstep.SetPaused(false);
        mFootstep.Restart();
        mLastFootstep = 0.5f;
    }

    // Update position of FPS model relative to actor position
    const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
    Vector3 modelPos = GetPosition();
    modelPos += GetForward() * modelOffset.x;
    modelPos += GetRight() * modelOffset.y;
    modelPos.z += modelOffset.z;
    mFPSModel->SetPosition(modelPos);
    // Initialize rotation to actor rotation
    Quaternion q = GetRotation();
    // Rotate by pitch from camera
    q = Quaternion::Concatenate(q, Quaternion(GetRight(), mCameraComp->GetPitch()));
    mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const InputState& code)
{
    float forwardSpeed = 0.0f;
    float strafeSpeed = 0.0f;
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
        strafeSpeed -= 400.0f;
    }
    if (code.Keyboard.GetKeyValue(SDL_SCANCODE_D))
    {
        strafeSpeed += 400.0f;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetStrafeSpeed(strafeSpeed);

    // Mouse movement
    // Assume mouse movement is usually between -500 and +500
    const int maxMouseSpeed = 500;
    // Rotation/sec at maximum speed
    const float maxAngularSpeed = GameMath::Pi * 8;
    float angularSpeed = 0.0f;
    if (code.Mouse.GetPosition().x != 0)
    {
        // Convert to ~[-1.0, 1.0]
        angularSpeed = code.Mouse.GetPosition().x / maxMouseSpeed;
        // Multiply by rotation/sec
        angularSpeed *= maxAngularSpeed;
    }
    mMoveComp->SetAngularSpeed(angularSpeed);

    // Compute pitch
    const float maxPitchSpeed = GameMath::Pi * 8;
    float pitchSpeed = 0.0f;
    if (code.Mouse.GetPosition().y != 0)
    {
        // Convert to ~[-1.0, 1.0]
        pitchSpeed = code.Mouse.GetPosition().y / maxMouseSpeed;
        pitchSpeed *= maxPitchSpeed;
    }
    mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetFootstepSurface(float value)
{
    // Pause here because the way I setup the parameter in FMOD
    // changing it will play a footstep
    mFootstep.SetPaused(true);
    mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
    mMeshComp->SetVisible(visible);
}
