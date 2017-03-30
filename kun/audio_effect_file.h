#pragma once

#include "audio_api.h"
#include <stdio.h>
#include <string>


class SoundEffect;


class AudioEffectFile
{
	friend SoundEffect;


public:

	AudioEffectFile (const std::string& filename);
	~AudioEffectFile ();
	ALuint							GetBuffer () const;			// @TODO: In future, hide the buffer id in some agnostic data type.
	float							GetLengthSeconds () const;


private:

	int								channel_num_;				// @NOTE: Just in case.
	int								sample_rate_;				// @NOTE: Just in case.

	ALuint							buffer_id_;
	float							length_seconds_;
	int								length_samples_;			// @NOTE: Just in case.
};

