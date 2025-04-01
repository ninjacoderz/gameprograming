// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "CameraActor.h"
#include "MoveComponent.h"
#include <SDL3/SDL_scancode.h>
#include "Renderer.h"
#include "Game.h"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Compute new camera from this actor
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const SDL_Scancode code)
{
	SDL_Log("CameraActor::ActorInput %d", code);
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	// wasd movement
	if (code == SDL_SCANCODE_W)
	{
		forwardSpeed += 300.0f;
	}
	if (code == SDL_SCANCODE_S)
	{
		forwardSpeed -= 300.0f;
	}
	if (code == SDL_SCANCODE_A)
	{
		angularSpeed -= GameMath::TwoPi;
	}
	if (code == SDL_SCANCODE_D)
	{
		angularSpeed += GameMath::TwoPi;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}
