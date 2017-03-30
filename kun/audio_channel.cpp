#include "audio_channel.h"
#include "sound_base.h"


AudioChannel::AudioChannel (ALuint const source)
	: source_ (source), base_ (NULL), is_used_ (false), is_sleeping_ (false)
{
	// @TODO: It would be more coherent to create source here, since it is deleted in destructor. Or opposite.
}


AudioChannel::~AudioChannel ()
{
	alDeleteSources (1, &source_);
}


void AudioChannel::Reserve (Sound* const sound_base)
{
	is_used_ = true;
	base_ = sound_base;
	SetPriority (sound_base->GetPriority ());
}


void AudioChannel::Release ()
{
	is_used_ = false;
	alSourcei (source_, AL_BUFFER, 0);
}


void AudioChannel::Cancel ()
{
	base_->Cancel ();
}

void AudioChannel::PriorityChanged ()
{

}

