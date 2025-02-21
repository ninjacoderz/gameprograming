
#pragma once
#include "Component.h"
#include <vector>
#include "GameMath.h"
#include "Actor.h"

class MoveComponent: public Component 
{
    public:
        MoveComponent(class Actor* owner, int updateOrder = 10);
        void Update(float deltaTime) override;
        void AddForce(Vector2 force);

        float GetAnularSpeed() const { return mAngularSpeed; }
        float GetMass() const { return mMass; }
        Vector2 GetVelocity() const { return mVelocity; }
        Vector2 GetSunOfForces () const { return mSumOfForces; }

        void SetAngularSpeed(float spped) { mAngularSpeed = spped; }
        void SetMass( float mass ) { mMass = mass; }
        void SetVelocity (Vector2 velocity) {mVelocity = velocity;}


    private: 
        float mAngularSpeed;
        float mMass;
	    Vector2 mVelocity;
        Vector2 mSumOfForces;
};