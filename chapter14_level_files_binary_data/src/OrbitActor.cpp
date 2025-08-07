//
// Created by Binh Nguyen Thanh on 26/05/2025.
//

#include "OrbitActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "OrbitCamera.h"
#include "MoveComponent.h"

OrbitActor::OrbitActor(Game* game)
    :Actor(game)
{
    mMeshComp = new MeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
    SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const InputState &keyState)
{
    // Mouse rotation
    // Get relative movement from SDL
    float x = keyState.Mouse.GetPosition().x;
    float y = keyState.Mouse.GetPosition().y;
    // Only apply rotation if right-click is held
    if (keyState.Mouse.GetButtonValue(SDL_BUTTON_RIGHT) )
    {
        // Assume mouse movement is usually between -500 and +500
        const int maxMouseSpeed = 500;
        // Rotation/sec at maximum speed
        const float maxOrbitSpeed = GameMath::Pi * 8;
        float yawSpeed = 0.0f;
        if (x != 0)
        {
            // Convert to ~[-1.0, 1.0]
            yawSpeed = static_cast<float>(x) / maxMouseSpeed;
            // Multiply by rotation/sec
            yawSpeed *= maxOrbitSpeed;
        }
        mCameraComp->SetYawSpeed(-yawSpeed);

        // Compute pitch
        float pitchSpeed = 0.0f;
        if (y != 0)
        {
            // Convert to ~[-1.0, 1.0]
            pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
            pitchSpeed *= maxOrbitSpeed;
        }
        mCameraComp->SetPitchSpeed(pitchSpeed);
    }
}

void OrbitActor::SetVisible(bool visible)
{
    mMeshComp->SetVisible(visible);
}
