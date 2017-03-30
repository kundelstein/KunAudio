#pragma once

#include "manager_base.h"
#include "sound_event.h"
#include <stdio.h>
#include <list>


class Mixer;
class SoundEffect;
class SoundStream;


class MixerManager : public Manager<Mixer>
{
	friend Mixer;


public:

	MixerManager (Engine* const engine_ptr);
	~MixerManager ();

	void						ManagerUpdateMixer (const float dt);


private:

	void						ManagerQueueSoundEvent (const SoundEvent& event);
	void						ManagerCleanSoundEvent (Sound* const sound);


private:

	SoundEventList				sound_event_arr_;
};

