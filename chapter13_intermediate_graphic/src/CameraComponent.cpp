
//
// Created by Binh Nguyen Thanh on 24/05/2025.
//

#include "CameraComponent.h"

#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "AudioSystem.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
    :Component(owner, updateOrder)
{
}

void CameraComponent::SetViewMatrix(const Matrix4& viewMatrix) const {
    Game* game = mOwner->GetGame();
    game->GetRenderer()->SetViewMatrix(viewMatrix);
    game->GetAudioSystem()->SetListener(viewMatrix);
}
