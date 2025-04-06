#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

#include <SDL3/SDL_log.h>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
	,mSystem(nullptr)
	,mLowLevelSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{
    mSystem->unloadAll();
    mSystem->release();
    mSystem = nullptr;
    mLowLevelSystem = nullptr;
}

bool AudioSystem::Initialize()
{
    // Initialize debug logging
    FMOD::Debug_Initialize(
        FMOD_DEBUG_LEVEL_ERROR, // Log only errors
        FMOD_DEBUG_MODE_TTY // Output to stdout
    );

    FMOD_RESULT result;
    result = FMOD::Studio::System::create(&mSystem);
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }
    result = mSystem->initialize(
        512, // Max number of concurrent sounds
        FMOD_STUDIO_INIT_NORMAL, // Use default settings
        FMOD_INIT_NORMAL, // Use default settings
        nullptr // Usually null
    );
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }
    // Save the low-level system pointer
    result = mSystem->getCoreSystem(&mLowLevelSystem);

    // Load the master banks (strings first)
	// LoadBank("Assets/Master Bank.strings.bank");
	// LoadBank("Assets/Master Bank.bank");

    return true;
}

void AudioSystem::Update(float deltaTimee) {
    // Update FMOD
    mSystem->update();
}      
void AudioSystem::Shutdown()
{
    // Shutdown FMOD system
    if (mSystem)
    {
        mSystem->release();
    }
}
