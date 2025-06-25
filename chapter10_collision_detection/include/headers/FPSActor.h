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

    void SetFootstepSurface(float value);

    void SetVisible(bool visible);
private:
    class MoveComponent* mMoveComp;
    class AudioComponent* mAudioComp;
    class MeshComponent* mMeshComp;
    class FPSCamera* mCameraComp;

    class Actor* mFPSModel;
    SoundEvent mFootstep;
    float mLastFootstep;
};
