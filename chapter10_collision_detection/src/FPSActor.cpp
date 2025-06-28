//
// Created by Binh Nguyen Thanh on 24/05/2025.
//

#include "FPSActor.h"
#include "AudioComponent.h"
#include "MoveComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "BoxComponent.h"
#include "BallActor.h"
#include "BallMove.h"
#include "Mesh.h"
#include "PlaneActor.h"

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

    // Add a box component
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f),
		Vector3(25.0f, 25.0f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
}

void FPSActor::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);

    // Play the footstep if we're moving and haven't recently
    mLastFootstep -= deltaTime;
    if (!GameMath::NearZero(mMoveComp->GetForwardSpeed()) 
        && mLastFootstep <= 0.0f)
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

    FixCollisions();
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
    const float maxAngularSpeed = GameMath::Pi * 16;
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
    const float maxPitchSpeed = GameMath::Pi * 16;
    float pitchSpeed = 0.0f;
    if (code.Mouse.GetPosition().y != 0)
    {
        // Convert to ~[-1.0, 1.0]
        pitchSpeed = code.Mouse.GetPosition().y / maxMouseSpeed;
        pitchSpeed *= maxPitchSpeed;
    }
    mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::Shoot()
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = GetGame()->GetRenderer()->Unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = GetGame()->GetRenderer()->Unproject(screenPoint);
	// Get direction vector
	Vector3 dir = end - start;
	dir.Normalize();
	// Spawn a ball
	BallActor* ball = new BallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir*20.0f);
	// Rotate the ball to face new direction
	ball->RotateToNewForward(dir);
	// Play shooting sound
	mAudioComp->PlayEvent("event:/Shot");
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

void FPSActor::FixCollisions()
{
	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// Calculate all our differences
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = GameMath::Abs(dx1) < GameMath::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = GameMath::Abs(dy1) < GameMath::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = GameMath::Abs(dz1) < GameMath::Abs(dz2) ?
				dz1 : dz2;
			
			// Whichever is closest, adjust x/y position
			if (GameMath::Abs(dx) <= GameMath::Abs(dy) && GameMath::Abs(dx) <= GameMath::Abs(dz))
			{
				pos.x += dx;
			}
			else if (GameMath::Abs(dy) <= GameMath::Abs(dx) && GameMath::Abs(dy) <= GameMath::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}
}