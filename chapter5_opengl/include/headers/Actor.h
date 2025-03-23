
#pragma once
#include "GameMath.h"
#include <vector>

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
        void Update(float deltaTime);
        void UpdateComponents(float deltaTime);
        virtual void ActorInput(const uint8_t* keyState){};

        // Getters/setters
        const Vector2& GetPosition() const { return mPosition; }
        class Game* GetGame() { return mGame; }
        void SetPosition(const Vector2& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
        float GetScale() const { return mScale; }
        void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true;}
        float GetRotation() const { return mRotation; }
        void SetRotation(float rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }  
        State GetState() const { return mState; }
        void SetState(State state) { mState = state; }

        // Add/remove components
        void AddComponent(class Component* component);
        void RemoveComponent(class Component* component);
        Vector2 GetForward() const { return Vector2(GameMath::Cos(mRotation), GameMath::Sin(mRotation)); }
        void ComputeWorldTransform();
        const Matrix4& GetWorldTransform() const { return mWorldTransform; }
        
    private:
        State mState;
        // Transform
        Vector2 mPosition;
        float mScale;
        float mRotation;

        std::vector<class Component*> mComponents;
        class Game* mGame;

        Matrix4 mWorldTransform;
        bool mRecomputeWorldTransform;
          
};