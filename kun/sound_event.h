#pragma once

#include "sound_callback.h"
#include <stdio.h>
#include <list>


class Sound;


class SoundEvent
{
public:

	SoundEvent (Sound* const sound, const SoundState state, const SoundCallback& callback)
		: sound_ (sound), state_ (state), callback_ (callback)
	{
	}


	void Execute () const
	{
		callback_ (sound_, state_);
	}


	void CleanIfEqual (Sound* const sound)
	{
		if (sound_ == sound)
		{
			sound_ = NULL;
		}
	}
	
	
public:

	Sound*						sound_;					// This is not const because it can be set to NULL after deleting the sound.
	SoundState					state_;


private:

	SoundCallback				callback_;
};


typedef std::list <SoundEvent> SoundEventList;

