#include "sound_stream.h"
#include "audio_channel_atlas.h"
#include "audio_channel.h"
#include "mixer.h"
#include "mathlib.h"
#include "debug.h"
#include "types.h"
#include <string>
#include <iostream>
#include <string.h>
#include <stb/stb_vorbis.h>


SoundStream::SoundStream (const std::string& filename, const int priority, Mixer* const mixer)
{
	SetPriority (priority);

	pause_time_samples_ = 0;

	is_streaming_ = false;
	channel_num_ = 0;
	sample_rate_ = 0;
	format_ = AL_FORMAT_STEREO16;
	buffer_length_ = 0;
	total_samples_left_ = 0;

	ZeroMyMemory (pcm_arr_, sizeof (pcm_arr_));		// @TODO: Not really necessary.

	alGetError ();									// @NOTE: Just in case clear the error buffer.

	// Initialize the buffers:
	alGenBuffers (NUM_BUFFERS, buffer_arr_);
	if (alGetError () != AL_NO_ERROR)
	{
		DBG_ERRORMSG ("OpenAL Error: Buffer could not be generated.\n");
		return;
	}

	is_loaded_ = true;

	// Load the first batch:

	vorbis_ = stb_vorbis_open_filename (filename.c_str (), NULL, NULL);

	if (!vorbis_)
		return;


	// Get file info:
	const stb_vorbis_info info = stb_vorbis_get_info (vorbis_);
	channel_num_ = info.channels;
	sample_rate_ = info.sample_rate;
	format_ = (channel_num_ == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	buffer_length_ = BUFFER_SIZE / channel_num_;		// @NOTE: duration = BUFFER_SIZE / (sound.freqency * sound.channels * sound.bitsPerChannel / 8)

	length_seconds_ = stb_vorbis_stream_length_in_seconds (vorbis_);
	length_samples_ = stb_vorbis_stream_length_in_samples (vorbis_);

	SoundAddToMixer (mixer);
}


SoundStream::~SoundStream ()
{
	SoundRemoveFromMixer ();

	// Deinitialize buffers:
	alDeleteBuffers (NUM_BUFFERS, buffer_arr_);
	stb_vorbis_close (vorbis_);
}


void SoundStream::InternalAttachSource ()
{
	alSourcei (channel_ptr_->source_, AL_SOURCE_TYPE, AL_STREAMING);
}


void SoundStream::InternalUpdateLoop ()
{
	// @NOTE: In case of stream, the loop is handled by Update, so we do not need to do anything here.
}


void SoundStream::InternalPlay ()
{
	SetSeekRewind ();
	Launch ();
}


void SoundStream::InternalPlayFromSeconds (const float seconds)
{
	SetSeekSeconds (seconds);
	Launch ();
}


void SoundStream::InternalStop ()
{
	const ALuint source = channel_ptr_->source_;

	is_streaming_ = false;

	alSourceStop (source);

	ALint queued;
	alGetSourcei (source, AL_BUFFERS_QUEUED, &queued);

	while (queued--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers (source, 1, &buffer);
	}
}


void SoundStream::InternalRemember ()
{
	pause_time_samples_ = GetSeekSamples (is_loop_);
}


void SoundStream::InternalResume ()
{
	SetSeekSamples (pause_time_samples_);
	Launch ();
}


void SoundStream::InternalRewind ()
{
	InternalStop ();
	InternalPlay ();
}


bool SoundStream::InternalUpdate (const float dt)
{
	DBG_ASSERTMSG (state_ == PlayingState::PLAYING, "SoundStream Update error.");

	const ALuint source = channel_ptr_->source_;

	//DBG_MSG ("Updating buffer... samples: %d", stb_vorbis_get_sample_offset(vorbis));
	//DBG_MSG ("TotalSamplesLeft: %d", total_samples_left_);


	ALint processed = 0;
	alGetSourcei (source, AL_BUFFERS_PROCESSED, &processed);

	while (processed--)
	{
		ALuint buffer = 0;
		alSourceUnqueueBuffers (source, 1, &buffer);

		if (Fill (buffer))
		{
			alSourceQueueBuffers (source, 1, &buffer);
		}
		else
		{
			is_streaming_ = false;
			break;
		}
	}


	ALint state;
	alGetSourcei (source, AL_SOURCE_STATE, &state);

	if (state == AL_STOPPED && !is_streaming_)
	{
		return false;
	}
	else if (state != AL_PLAYING)
	{
		alSourcePlay (source);
	}

	return true;
}


void SoundStream::SetSeekSamples (const int sample)
{
	if (sample < 0)
	{
		SetSeekRewind ();
		return;
	}

	stb_vorbis_seek (vorbis_, sample);
	total_samples_left_ = (stb_vorbis_stream_length_in_samples (vorbis_) - sample) * channel_num_;
}


void SoundStream::SetSeekSeconds (const float second)
{
	SetSeekSamples (static_cast<int>(second * sample_rate_));
}


void SoundStream::SetSeekRewind ()
{
	stb_vorbis_seek_start (vorbis_);
	total_samples_left_ = stb_vorbis_stream_length_in_samples (vorbis_) * channel_num_;
}


int SoundStream::GetSeekSamples (const bool loop) const
{
	const ALuint source = channel_ptr_->source_;

	ALint queued;
	alGetSourcei (source, AL_BUFFERS_QUEUED, &queued);

	ALint processed;
	alGetSourcei (source, AL_BUFFERS_PROCESSED, &processed);

	ALint buff_time;
	alGetSourcei (source, AL_SAMPLE_OFFSET, &buff_time);

	int recalculated = stb_vorbis_get_sample_offset (vorbis_) + buff_time - (queued - processed) * buffer_length_;

	if (loop)
	{
		if (recalculated < 0)
		{
			recalculated = length_samples_ + recalculated;
		}
	}

	return	recalculated;
}


float SoundStream::GetSeekSeconds (const bool loop) const
{
	return GetSeekSamples (loop) / (float) sample_rate_;
}


bool SoundStream::Fill (const ALuint buffer)
{
	int  size = 0;

	while (size < BUFFER_SIZE)
	{
		const int read_samples = stb_vorbis_get_samples_short_interleaved (vorbis_, channel_num_, pcm_arr_ + size, BUFFER_SIZE - size);

		if (read_samples > 0)
		{
			size += read_samples * channel_num_;
		}
		else
		{
			if (!is_loop_)
				break;

			stb_vorbis_seek_start (vorbis_);
			total_samples_left_ = stb_vorbis_stream_length_in_samples (vorbis_) * channel_num_;
		}
	}

	if (size == 0)
		return false;

	alBufferData (buffer, format_, pcm_arr_, size * sizeof (ALshort), sample_rate_);
	total_samples_left_ -= size;

	return true;
}


bool SoundStream::Launch ()
{
	const ALuint source = channel_ptr_->source_;

	for (int i = 0; i < NUM_BUFFERS; ++i)
	{
		if (!Fill (buffer_arr_[i]))
			return false;
	}

	alSourceQueueBuffers (source, NUM_BUFFERS, buffer_arr_);
	alSourcePlay (source);

	is_streaming_ = true;

	return true;
}

