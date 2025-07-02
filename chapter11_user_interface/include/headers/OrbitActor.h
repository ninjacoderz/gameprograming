//
// Created by Binh Nguyen Thanh on 26/05/2025.
//

#pragma once
#include "Actor.h"

class OrbitActor : public Actor
{
public:
    OrbitActor(class Game* game);

    void ActorInput(const InputState &keyState) override;

    void SetVisible(bool visible);
private:
    class OrbitCamera* mCameraComp;
    class MeshComponent* mMeshComp;
};