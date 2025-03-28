#pragma once
#include "MoveComponent.h"
#include <SDL3/SDL.h>

class InputComponent : public MoveComponent
{
public:
    InputComponent(class Actor* owner);
    void ProcessInput(const SDL_Scancode code ) override;

    float GetMaxForward() const { return mMaxForwardSpeed; }
	float GetMaxAngular() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

    void SetForwardKey(int key) { mForwardKey = key; }
    void SetBackKey(int key) { mBackKey = key; }
    void SetClockwiseKey(int key) { mClockwiseKey = key; }
    void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
    void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
    void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
private:
    int mForwardKey;
    int mBackKey;
    int mClockwiseKey;
    int mCounterClockwiseKey;
    float mMaxForwardSpeed;
    float mMaxAngularSpeed;
};