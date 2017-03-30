#pragma once

#include "engine_config.h"
#include "managed_base.h"
#include "cluster.h"
#include "mixer_manager.h"
#include "debug.h"
#include <stdio.h>
#include <list>


class SceneRunner;
class Engine;
class SoundEffect;
class SoundStream;
class Sound;
class SoundEvent;


// Set active works like this: If I use Mixer.SetActive(false) mixer itself is inactive and it doesn't affect the state IsActive
// of individual Effects. But the sounds won't be played.

class Mixer : public Managed<MixerManager, Mixer>
{
	friend SceneRunner;
	friend Engine;
	friend MixerManager;
	friend SoundEffect;
	friend SoundStream;
	friend Sound;
	friend Managed;


	typedef std::list <Sound*> SoundList;


public:

	Mixer (const int priority = 0, const bool add = true);
	~Mixer ();

	void							SetReferenceDistance (const float val);
	float							GetReferenceDistance () const;
	void							SetRolloffFactor (const float val);
	float							GetRolloffFactor () const;
	void							SetMaxDistance (const float val);
	float							GetMaxDistance () const;
	void							SetRelative (const bool relative);
	bool							GetRelative () const;
	void							SetVolume (const float volume);
	float							GetVolume () const;


private:

	void							Update (const float dt);

	void							QueueSoundEvent (const SoundEvent& event);
	void							CleanEventBySound (Sound* const sound);


private:

	void							EnableInternal ();
	void							DisableInternal ();


private:

	Cluster<Sound>					sound_cluster_;
	SoundList						paused_arr_;			// This is used to remember what was played when disabling the mixer.
	float							reference_distance_;
	float							rolloff_factor_;
	float							max_distance_;
	float							volume_;
	bool							is_relative_;

};

