#include "SoundEvent.h"
#include "AudioSystem.h"

SoundEvent::SoundEvent(class AudioSystem* system, unsigned int id)
	:mSystem(system)
	,mID(id)
{
}

SoundEvent::SoundEvent()
	:mSystem(nullptr)
	,mID(0)
{
}

void SoundEvent::SetPaused(bool pause)
{
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->setPaused(pause);
    }
}

bool SoundEvent::IsValid()
{
    return mID != 0;
}
void SoundEvent::Restart()
{
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
        event->start();
    }
}
void SoundEvent::Stop(bool allowFadeOut)
{
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->stop(allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
    }
}
void SoundEvent::SetVolume(float volume)
{
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->setVolume(volume);
    }
}
void SoundEvent::SetPitch(float pitch)
{
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->setPitch(pitch);
    }
}
void SoundEvent::SetParameter(const std::string& name, float value)
{
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->setParameterByName(name.c_str(), value);
    }
}

bool SoundEvent::GetPaused() const
{
    bool retVal = false;
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->getPaused(&retVal);
    }
    return retVal;
}

float SoundEvent::GetVolume() const
{
    float retVal = 0.0f;
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->getVolume(&retVal);
    }
    return retVal;
}

float SoundEvent::GetPitch() const
{
    float retVal = 0.0f;
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->getPitch(&retVal);
    }
    return retVal;
}

float SoundEvent::GetParameter(const std::string& name) const
{
    float retVal = 0.0f;
    auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
    if (event)
    {
        event->getParameterByName(name.c_str(), &retVal);
    }
    return retVal;
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
