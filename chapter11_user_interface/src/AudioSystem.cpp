#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

#include <SDL3/SDL_log.h>
#include "SoundEvent.h"

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
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

    return true;
}

void AudioSystem::LoadBank(const std::string& name)
{
    // Check if the bank is already loaded
    auto iter = mBanks.find(name);
    if (iter != mBanks.end())
    {
        SDL_Log("Bank %s is already loaded", name.c_str());
        return;
    }
    FMOD::Studio::Bank* bank = nullptr;
    FMOD_RESULT result = mSystem->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    
    const int maxPathLength = 512;
    if (result == FMOD_OK)
    {   
        // Add  the bank to the list of loaded banks
        mBanks.emplace(name, bank);
        bank->loadSampleData();

        int numEvents = 0;
        bank->getEventCount(&numEvents);
        if(numEvents > 0)
        {
            std::vector<FMOD::Studio::EventDescription*> events(numEvents);
            bank->getEventList(events.data(), numEvents, &numEvents);
            char eventName[maxPathLength];
            for (int i = 0; i < numEvents; ++i)
            {
                FMOD::Studio::EventDescription* ed = events[i];
                
                ed->getPath(eventName, maxPathLength, nullptr);
                mEvents.emplace(eventName, ed);
            }
        }

        // Get the number of buses in this bank
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// Get list of buses in this bank
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				// Get the path of this bus (like bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// Add to buses map
				mBuses.emplace(busName, bus);
			}
		}
    }
    else
    {
        SDL_Log("Failed to load bank %s: %s", name.c_str(), FMOD_ErrorString(result));
        return;
    }

}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
    unsigned int retID = 0;
    auto iter = mEvents.find(name);
    if (iter != mEvents.end())
    {
        FMOD::Studio::EventInstance* eventInstance = nullptr;
        iter->second->createInstance(&eventInstance);
        if (eventInstance)
        {
            eventInstance->start();
            sNextID++;
            retID = sNextID;
            mEventInstances.emplace(retID, eventInstance);
        }
    }
    else
    {
        SDL_Log("Failed to play event %s: Event not found", name.c_str());
    }
    return SoundEvent(this, retID);
}

void AudioSystem::UnloadBank(const std::string& name)
{
    auto iter = mBanks.find(name);
    if (iter != mBanks.end())
    {
        iter->second->unload();
        mBanks.erase(iter);
    }
}

void AudioSystem::UnloadAllBanks()
{
    for (auto& pair : mBanks)
    {
        pair.second->unload();
    }
    mBanks.clear();
}

void AudioSystem::Update(float deltaTimee) {
    // Update FMOD
    std::vector<unsigned int> done;

    for(auto & iter : mEventInstances)
    {
        FMOD::Studio::EventInstance* e = iter.second;
        FMOD_STUDIO_PLAYBACK_STATE state;
        e->getPlaybackState(&state);
        if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
        {
            e->release();
            done.emplace_back(iter.first);
        }
    }

    for(auto id : done)
    {
        mEventInstances.erase(id);
    }
    mSystem->update();
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		event = iter->second;
	}
	return event;
}

void AudioSystem::Shutdown()
{
    // Shutdown FMOD system
    if (mSystem)
    {
        mSystem->release();
    }
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		// Convert from our coordinates (+x forward, +y right, +z up)
		// to FMOD (+z forward, +x right, +y up)
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
    Matrix4 invView = viewMatrix;
    invView.Invert();
    FMOD_3D_ATTRIBUTES listener;
    // Set position, forward, up
    listener.position = VecToFMOD(invView.GetTranslation());
    // In the inverted view, third row is forward
    listener.forward = VecToFMOD(invView.GetZAxis());
    // In the inverted view, second row is up
    listener.up = VecToFMOD(invView.GetYAxis());
    // Set velocity to zero (fix if using Doppler effect)
    listener.velocity = {0.0f, 0.0f, 0.0f};
    // Send to FMOD
    mSystem->setListenerAttributes(0, &listener);
}

bool SoundEvent::Is3D() const
{
    bool retVal = false;
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        // Get the event description
        FMOD::Studio::EventDescription* ed = nullptr;
        event->getDescription(&ed);
        if (ed)
        {
            ed->is3D(&retVal);
        }
    }
    return retVal;
}

void SoundEvent::Set3DAttributes(const Matrix4& worldTrans)
{
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        FMOD_3D_ATTRIBUTES attr;
        // Set position, forward, up
        attr.position = VecToFMOD(worldTrans.GetTranslation());
        // In world transform, first row is forward
        attr.forward = VecToFMOD(worldTrans.GetXAxis());
        // Third row is up
        attr.up = VecToFMOD(worldTrans.GetZAxis());
        // Set velocity to zero (fix if using Doppler effect)
        attr.velocity = { 0.0f, 0.0f, 0.0f };
        event->set3DAttributes(&attr);
    }
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string & name) const
{
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);
    SDL_Log("Set bus volume %s to %lu", name.c_str(), mBuses.size());
	if (iter != mBuses.end())
	{
        float currentVolume = 0.0f;
        iter->second->getVolume(&currentVolume);
        SDL_Log("Set bus volume %s from %f to %f", name.c_str(), currentVolume, volume);
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string & name, bool pause)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}
