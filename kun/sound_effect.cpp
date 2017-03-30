#include "sound_effect.h"
#include "audio_channel_atlas.h"
#include "audio_api.h"
#include "audio_channel.h"
#include "audio_effect_file.h"

#include "mixer.h"
#include "mathlib.h"
#include "debug.h"


SoundEffect::SoundEffect (AudioEffectFile* const sound_file, const int priority, Mixer* const mixer)
	: sound_file_ptr_ (sound_file)
{
	pause_time_samples_ = 0;

	length_seconds_ = sound_file_ptr_->GetLengthSeconds ();

	is_loaded_ = true;

	SetPriority (priority);

	SoundAddToMixer (mixer);
}


SoundEffect::~SoundEffect ()
{
	SoundRemoveFromMixer ();
}


void SoundEffect::InternalAttachSource ()
{
	const ALint source = channel_ptr_->source_;

	alSourcei (source, AL_SOURCE_TYPE, AL_STATIC);
	alSourcei (source, AL_BUFFER, sound_file_ptr_->GetBuffer ());

	alSourcei (channel_ptr_->source_, AL_LOOPING, is_loop_);
}


void SoundEffect::InternalUpdateLoop ()
{
	if (!channel_ptr_)
		return;

	alSourcei (channel_ptr_->source_, AL_LOOPING, is_loop_);
}


void SoundEffect::InternalPlay ()
{
	alSourcePlay (channel_ptr_->source_);
}


void SoundEffect::InternalPlayFromSeconds (const float seconds)
{
	alSourcef (channel_ptr_->source_, AL_SEC_OFFSET, seconds);
	InternalPlay ();
}


void SoundEffect::InternalStop ()
{
	alSourceStop (channel_ptr_->source_);
}


void SoundEffect::InternalRemember ()
{
	alGetSourcei (channel_ptr_->source_, AL_SAMPLE_OFFSET, &pause_time_samples_);
}


void SoundEffect::InternalResume ()
{
	alSourcei (channel_ptr_->source_, AL_SAMPLE_OFFSET, pause_time_samples_);
	InternalPlay ();
}


void SoundEffect::InternalRewind ()
{
	alSourceRewind (channel_ptr_->source_);
}


// @NOTE: That kind of update is unfortunate if its role is to loop sound as a part of the signal.
//        In such circumstances the sound will be delayed a bit.
bool SoundEffect::InternalUpdate (const float dt)
{
	DBG_ASSERTMSG (state_ == PlayingState::PLAYING, "Sound Effect Update error.");

	const ALint source = channel_ptr_->source_;
	ALint state;
	alGetSourcei (source, AL_SOURCE_STATE, &state);

	if (state == AL_STOPPED)
	{
		return false;

		// @NOTE: The code below generates small delays due Update being 1/60th the second. 
		//        Solution would be to do separate sound thread on high frequency (1/120 fps). 
		// @NOTE: If we need FINISH callback, we will need something else.
		/*
		if (is_loop_)
		{
			InternalPlay ();
		}
		else
		{
			return false;
		}
		*/
	}
	else if (state != AL_PLAYING)
	{
		alSourcePlay (source);
	}

	return true;
}

