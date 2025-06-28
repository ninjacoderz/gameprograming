//
// Created by Binh Nguyen Thanh on 24/05/2025.
//

#pragma once
#include "Actor.h"
#include "SoundEvent.h"


class FPSActor : public Actor {
public:
    FPSActor(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const InputState &keyState) override;

    void Shoot();

    void SetFootstepSurface(float value);

    void SetVisible(bool visible);

    void FixCollisions();
private:
    class MoveComponent* mMoveComp;
    class AudioComponent* mAudioComp;
    class MeshComponent* mMeshComp;
    class FPSCamera* mCameraComp;
    class BoxComponent* mBoxComp;
    class Actor* mFPSModel;
    SoundEvent mFootstep;
    float mLastFootstep;
};
