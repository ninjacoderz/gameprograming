//
// Created by Binh Nguyen Thanh on 24/05/2025.
//

#include "FPSCamera.h"
#include "Actor.h"
FPSCamera::FPSCamera(class Actor *owner) : CameraComponent(owner),
mPitch(0.0f),
mMaxPitch(GameMath::PiOver2),
mPitchSpeed(0.01f) {

}

void FPSCamera::Update(float deltaTime) {
    CameraComponent::Update(deltaTime);
    Vector3 cameraPos = mOwner->GetPosition();
    mPitch += mPitchSpeed * deltaTime;
    mPitch = GameMath::Clamp(mPitch, -mMaxPitch, mMaxPitch);

    Quaternion q(mOwner->GetRight(), mPitch);

    Vector3 viewForward = Vector3::Transform(mOwner->GetForward(), q);
    Vector3 target = cameraPos + viewForward * 100.0f;

    Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    SetViewMatrix(view);
}
