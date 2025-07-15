#pragma once

#include <string>
#include "AudioSystem.h"
#include "GameMath.h"

class SoundEvent {
public:
    SoundEvent();
    bool IsValid();
    bool Is3D() const;
    void Set3DAttributes(const Matrix4& worldTrans);
    
    // Restart event from beginning
    void Restart();
    void Stop(bool allowFadeOut = true);
    void SetPaused(bool pause);
    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetParameter(const std::string& name, float value);

    bool GetPaused() const;
    float GetVolume() const;
    float GetPitch() const;
    float GetParameter(const std::string& name) const;

protected:
    friend class AudioSystem;
    SoundEvent(class AudioSystem* system, unsigned int id);
private:
    class AudioSystem* mSystem;
    unsigned int mID;
};