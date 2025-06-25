//
// Created by Binh Nguyen Thanh on 24/05/2025.
//


#include "CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:

    FPSCamera(class Actor* owner);
    void Update(float deltaTime) override;

    float GetPitch() const {
        return mPitch;
    }

    float GetMaxPitch() const {
        return mMaxPitch;
    }

    float GetPitchSpeed() const {
        return mPitchSpeed;
    }

    void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
    void SetMaxPitch(float pitch) { mMaxPitch = pitch; }

private:
    float mPitch;
    float mMaxPitch;
    float mPitchSpeed;
};

