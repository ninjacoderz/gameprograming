#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "SoundEvent.h"
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
    void ProcessInput(SDL_Event *event);
    class AudioSystem* GetAudioSystem() { return mAudioSystem; }
    void HandleKeyPress(int key);
private:
    void UpdateGame();
    void GenerateOutput();
    virtual void LoadData();
    void UnloadData();
    

    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;
    
    class Renderer* mRenderer;

    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    // Game-specific code
	class CameraActor* mCameraActor;
    class AudioSystem* mAudioSystem;

    class SoundEvent mMusicEvent;
	class SoundEvent mReverbSnap;
};