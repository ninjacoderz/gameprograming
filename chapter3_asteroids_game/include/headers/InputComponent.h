#pragma once
#include "MoveComponent.h"

class InputComponent : public MoveComponent
{
public:
	// Lower update order to update first
	InputComponent(class Actor* owner, float mass);

	void ProcessInput(const SDL_Scancode code ) override;
	
	// Getters/setters for private variables
	float GetMaxAngular() const { return mMaxAngularSpeed; }
    float GetMaxForwardSpeed() const { return mMaxForwardSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

	
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
    void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
private:
    // The maximum forward/angular speeds
    float mMaxForwardSpeed;
    float mMaxAngularSpeed;
    // Keys for forward/back movement
    int mForwardKey;
    int mBackKey;
    // Keys for angular movement
    int mClockwiseKey;
    int mCounterClockwiseKey;
};
