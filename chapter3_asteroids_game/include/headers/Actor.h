
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
        void Update(float deltaTime);
        void UpdateComponents(float deltaTime);
        virtual void UpdateActor(float deltaTime);

        // Getters/setters
        const Vector2& GetPosition() const { return mPosition; }
        class Game* GetGame() { return mGame; }
        virtual void SetPosition(const Vector2& pos) { mPosition = pos; }
        float GetScale() const { return mScale; }
        void SetScale(float scale) { mScale = scale; }
        float GetRotation() const { return mRotation; }
        void SetRotation(float rotation) { mRotation = rotation; }  
        State GetState() const { return mState; }
        void SetState(State state) { mState = state; }

        // Add/remove components
        void AddComponent(class Component* component);
        void RemoveComponent(class Component* component);

        Vector2 GetForward() const { return Vector2(GameMath::Cos(mRotation), -GameMath::Sin(mRotation)); }
        
        void ProcessInput(const SDL_Scancode code );
        virtual void ActorInput(const SDL_Scancode code) {};

    protected:
        State mState;
        // Transform
        Vector2 mPosition;
        float mScale;
        float mRotation;

        std::vector<class Component*> mComponents;

        class Game* mGame;
};