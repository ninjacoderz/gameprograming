
#pragma once
#include "GameMath.h"
#include <vector>
#include "InputSystem.h"
class Actor
{
    public:
        enum State
        {
            EActive,
            EPaused,
            EDead
        };
        explicit Actor(class Game* game);
        virtual ~Actor();

        // Update a function called from Game (not overridable)
        virtual void Update(float deltaTime);
        virtual void UpdateActor(float deltaTime) {};
        void UpdateComponents(float deltaTime);
        void ProcessInput(const InputState& state);
        virtual void ActorInput(const InputState& keyState) {};
        // Getters/setters
        const Vector3& GetPosition() const { return mPosition; }
        class Game* GetGame() const { return mGame; }
        void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
        float GetScale() const { return mScale; }
        void SetScale(const float scale) { mScale = scale; mRecomputeWorldTransform = true;}
        Quaternion GetRotation() const { return mRotation; }
        
        void SetRotation(const Quaternion rotation) {
            mRotation = rotation; 
            mRecomputeWorldTransform = true; 
        }
        State GetState() const { return mState; }
        void SetState(const State state) { mState = state; }
        void RotateToNewForward(const Vector3& forward);

        // Add/remove components
        void AddComponent(class Component* component);
        void RemoveComponent(class Component* component);

        Vector3 GetForward() const { 
            return Vector3::Transform(Vector3::UnitX, mRotation); 
        }
        Vector3 GetRight() const {
            return Vector3::Transform(Vector3::UnitY, mRotation);
        }

        // 3D graphics
        void ComputeWorldTransform();
        const Matrix4& GetWorldTransform() const { return mWorldTransform; }
        
    private:
        State mState;
        // Transform
        Vector3 mPosition;
        float mScale;
        Quaternion mRotation;

        std::vector<class Component*> mComponents;
        class Game* mGame;

        Matrix4 mWorldTransform;
        bool mRecomputeWorldTransform;
          
};