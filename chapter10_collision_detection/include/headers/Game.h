#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>

#include "FPSActor.h"
#include "OrbitActor.h"
#include "SoundEvent.h"
#include "SpriteComponent.h"

class Game {
public: 
    Game();
    // Initialize the game
    virtual bool Initialize();
    // Run the game loop
    void RunLoop();
    // Shutdown the game
    void Shutdown();
    // Add an actor to the game
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    // Get the renderer
    class Renderer* GetRenderer() { return mRenderer; }
    void ProcessInput();
    class AudioSystem* GetAudioSystem() { return mAudioSystem; }
    class PhysWorld* GetPhysWorld() { return mPhysWorld; }

    // Game-specific
    void RemovePlane(class PlaneActor* plane);
    void AddPlane(class PlaneActor* plane);
    std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
private:
    void UpdateGame();
    void GenerateOutput();
    virtual void LoadData();
    void UnloadData();
    void HandleKeyPress(int key);

    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;
    
    class Renderer* mRenderer;

    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    // Game-specific code
    class AudioSystem* mAudioSystem;
    class PhysWorld* mPhysWorld;

    class SoundEvent mMusicEvent;
	class SoundEvent mReverbSnap;

    class InputSystem* mInputSystem;
    FPSActor * mFPSActor;
    SpriteComponent * mCrosshair;

    class FollowActor* mFollowActor;
    OrbitActor * mOrbitActor;
    class SplineActor* mSplineActor;
    
    void ChangeCamera(int mode) const;

    std::vector<class PlaneActor*> mPlanes;
};
