#pragma once

#include "priority_element.h"
#include "audio_api.h"


class Sound;
class Engine;


// @TODO: Pack it a bit and use DOD, also use different pool type.
class AudioChannel : public Priorited<AudioChannel>
{
	friend Sound;
	friend Engine;
	friend Priorited;


public:

	AudioChannel (ALuint const source);
	~AudioChannel ();
	void							Reserve (Sound* const sound_base);
	void							Release ();
	void							Cancel ();


public:

	Sound*							base_;
	ALuint							source_;

	bool							is_used_;
	bool							is_sleeping_;


private:

	void							PriorityChanged ();		// public Priorited
};

