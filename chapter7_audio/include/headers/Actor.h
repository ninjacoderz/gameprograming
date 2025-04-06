
#pragma once
#include "GameMath.h"
#include <vector>
#include <SDL3/SDL.h>
class Actor
{
    public:
        enum State
        {
            EActive,
            EPaused,
            EDead
        };
        Actor(class Game* game);
        virtual ~Actor();

        // Update function called from Game (not overridable)
        virtual void Update(float deltaTime);
        virtual void UpdateActor(float deltaTime) {};
        void UpdateComponents(float deltaTime);
        void ProcessInput(const SDL_Scancode code);
        virtual void ActorInput(const SDL_Scancode keyState) {};
        // Getters/setters
        const Vector3& GetPosition() const { return mPosition; }
        class Game* GetGame() { return mGame; }
        void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
        float GetScale() const { return mScale; }
        void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true;}
        Quaternion GetRotation() const { return mRotation; }
        
        void SetRotation(Quaternion rotation) { 
            mRotation = rotation; 
            mRecomputeWorldTransform = true; 
        }
        State GetState() const { return mState; }
        void SetState(State state) { mState = state; }

        // Add/remove components
        void AddComponent(class Component* component);
        void RemoveComponent(class Component* component);
        Vector3 GetForward() const { 
            return Vector3::Transform(Vector3::UnitX, mRotation); 
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