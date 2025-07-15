//
// Created by Binh Nguyen Thanh on 26/05/2025.
//

#pragma once
#include "Actor.h"
#include "Game.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include "MeshComponent.h"

class FollowActor : public Actor
{
public:
    FollowActor(class Game* game);

    void ActorInput(const InputState& keyState) override;

    void SetVisible(bool visible) const;
private:
    class MoveComponent* mMoveComp;
    class FollowCamera* mCameraComp;
    class SkeletalMeshComponent* mMeshComp;

    bool mMoving;
};
