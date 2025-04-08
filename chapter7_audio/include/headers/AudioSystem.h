#pragma once

#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include <fmod_studio.hpp>
#include <fmod.hpp>

// Forward declaration of SoundEvent class

#include "GameMath.h"
class AudioSystem
{

public:
    AudioSystem(class Game* game);
    ~AudioSystem();
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void LoadBank(const std::string& name);
    void UnloadBank(const std::string& name);
    void UnloadAllBanks();
    class SoundEvent PlayEvent(const std::string& name);
    void SetListener(const Matrix4& viewMatrix);

    // Control buses
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
private:
    class Game* mGame;
    FMOD::Studio::System* mSystem;
    FMOD::System* mLowLevelSystem;
    // Tracks the next ID to use for event instances
	static unsigned int sNextID;

    // Map of loaded banks
    std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
    // Map of event name to EventDescription
    std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
    // // Map of event id to EventInstance
    std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
    // Map of buses
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
protected:
    // Map of bus name to Bus
    friend class SoundEvent;
    FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
};  
