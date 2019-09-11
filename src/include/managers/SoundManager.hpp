/*
    3KSC: A fighting game
    Copyright (C) 2018  Chaotic Games

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.

    You can contact Chaotic Games at: chaoticgamesdev@gmail.com
*/

#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#include <fmod/fmod.hpp>
#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_common.h>
#include <fmod/fmod_errors.h>

#include <string>
#include <vector>
#include <map>

#define FMOD_STUDIO_EVENT_CALLBACK_STOPPED 0x00000020

enum class SoundID{
    S_KIRA = 0,
    S_LUKA,
    S_MIYAGI,
    S_PLUP,
    S_RAWR,
    S_SPARKY,
    S_FOSFOS_STADIUM,
    S_MENU
};

struct Vector3 {
    float x;
    float y;
    float z;
};

class InputManager;
class SoundEvent;

class SoundManager{
public:
    static SoundManager& instance();
    SoundManager();
    ~SoundManager();

    void    createSoundEvent(const char* eventPath, const char* name, bool p_isEffectSound = true);
    void    playSound(const char* name);
    void    playMusic(const char* name);
    void    pauseAll();
    void    unPauseAll();

    void    update();

    void    loadBanks();
    void    loadBank(SoundID p_bank);
    void    loadEvents(SoundID p_bank);

    void    modifyParameter(const char* name, float num, const char* parameter, bool p_isEffectSound = true);
    bool    isPlaying(const char* p_name, bool p_checkUltimate = true);
    void    stopAll();

    void    setMusicVolume(float p_volume);
    void    setEffectVolume(float p_volume);

private:
    InputManager*           m_inputManager;

    FMOD_RESULT             m_Result;
    FMOD::Studio::System*   m_system;
    FMOD::System*           m_lowLevelSystem;
    
    FMOD::Studio::Bank*     m_masterBank;
    FMOD::Studio::Bank*     m_stringsBank;
    FMOD::Studio::Bank*     m_musicBank;

    bool                    m_soundEffectPlaying;
    float                   m_musicVolume;
    float                   m_effectVolume;
    bool                    m_soundMute;

    std::map<const char*, SoundEvent*>           m_musicEvents;
    std::map<const char*, SoundEvent*>           m_effectEvents;
    std::map<const char*, SoundEvent*>::iterator m_iterator;

    std::vector<SoundEvent*>    m_mutedSounds;

    void    updateSounds();
};

class SoundEvent{
public:
    SoundEvent(FMOD::Studio::EventDescription* p_eventDescription);
    ~SoundEvent();

    void start();
    void stop();
    void pause();
    void unPause();
    void setVolume(float p_volume);
    bool isPlaying();

    void modifyParameter(float num, const char* parameter);

    FMOD::Studio::EventInstance*    getEventInstance();

protected:
    FMOD::Studio::EventInstance*    m_soundInstance;
};

#endif