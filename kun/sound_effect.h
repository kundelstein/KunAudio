#pragma once

#include "engine_config.h"
#include "sound_event.h"
#include "sound_base.h"
#include "audio_api.h"
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>


class AudioEffectFile;
class Mixer;
class AudioChannel;


class SoundEffect : public Sound
{
	friend Mixer;
	friend AudioChannel;


private:

	bool							InternalUpdate (const float dt);
	void							InternalPlay ();
	void							InternalPlayFromSeconds (const float seconds);
	void							InternalStop ();
	void							InternalRewind ();
	void							InternalRemember ();
	void							InternalResume ();

	void							InternalAttachSource ();
	void							InternalUpdateLoop ();


private:

	typedef std::set<Sound*> SoundEffectList;


private:

	AudioEffectFile*				sound_file_ptr_;
	int								pause_time_samples_;


public:

	SoundEffect (AudioEffectFile* const sound_file, const int priority, Mixer* const mixer);
	~SoundEffect ();
};

