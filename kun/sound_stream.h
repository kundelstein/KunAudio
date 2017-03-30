#pragma once

#include "engine_config.h"
#include "sound_base.h"
#include "audio_api.h"
#include <cstdint>
#include <string>


class Mixer;
class AudioChannel;
class SoundStreamGuts;
struct stb_vorbis;


class SoundStream : public Sound
{
	friend Mixer;
	friend AudioChannel;


public:

	SoundStream (const std::string& filename, const int priority, Mixer* const mixer);
	~SoundStream ();


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


	void							SetSeekSamples (const int sample);
	void							SetSeekSeconds (const float second);
	void							SetSeekRewind ();
	int								GetSeekSamples (const bool loop) const;
	float							GetSeekSeconds (const bool loop) const;

	bool							Launch ();
	bool							Fill (const ALuint buffer);


private:

	enum Vars
	{
		NUM_BUFFERS = 8,			// 8
		BUFFER_SIZE = 4096 * 2		// *16
	};


	int								channel_num_;
	int								sample_rate_;

	int								buffer_length_;					// Playback buffer length.

	size_t							total_samples_left_;
	stb_vorbis*						vorbis_;
	ALuint							buffer_arr_[NUM_BUFFERS];
	ALshort							pcm_arr_[BUFFER_SIZE];
	ALenum							format_;

	int								pause_time_samples_;

	bool							is_streaming_;
};

