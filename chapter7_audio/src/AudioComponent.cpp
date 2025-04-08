
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "Game.h"

AudioComponent::AudioComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
    SoundEvent se = mOwner->GetGame()->GetAudioSystem()->PlayEvent(name);
    if (se.Is3D())
    {
        mEvents3D.emplace_back(se);
        se.Set3DAttributes(mOwner->GetWorldTransform());
    }
    else
    {
        mEvents2D.emplace_back(se);
    }
    return se;
}

void AudioComponent::OnUpdateWorldTransform()
{
    Matrix4 world = mOwner->GetWorldTransform();
    for (auto& se : mEvents3D)
    {
        if(se.IsValid() )
        {
            se.Set3DAttributes(world);
        }
    }
}

void AudioComponent::StopAllEvents()
{
    for (auto& se : mEvents2D)
    {
        se.Stop();
    }
    for (auto& se : mEvents3D)
    {
        se.Stop();
    }
}

AudioComponent::~AudioComponent()
{
	StopAllEvents();
}

void AudioComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	// Remove invalid 2D events
	auto iter = mEvents2D.begin();
	while (iter != mEvents2D.end())
	{
		if (!iter->IsValid())
		{
			iter = mEvents2D.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// Remove invalid 3D events
	iter = mEvents3D.begin();
	while (iter != mEvents3D.end())
	{
		if (!iter->IsValid())
		{
			iter = mEvents3D.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
