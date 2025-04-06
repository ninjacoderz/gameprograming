#pragma once

class AudioSystem
{

public:
    AudioSystem(class Game* game);
    ~AudioSystem();
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
private:
    class Game* mGame;
    FMOD::Studio::System* mSystem;
    FMOD::System* mLowLevelSystem;
    // Tracks the next ID to use for event instances
	static unsigned int sNextID;
};  
