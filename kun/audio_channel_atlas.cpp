#include "audio_channel_atlas.h"
#include "sound_base.h"
#include "engine_config.h"
#include "debug.h"
#include "audio_api.h"
#include "audio_channel.h"
#include <algorithm>


namespace
{
	enum MixerVars
	{
		CHANNEL_MAX = 32,
	};
}


AudioChannelPool::AudioChannelPool ()
{
	for (int i = 0; i < CHANNEL_MAX; ++i)
	{
		ALuint source;
		alGenSources (1, &source);
		if (alGetError () != AL_NO_ERROR)
			break;

		pool_.CreateSlot (new AudioChannel (source));
	}

	DBG_ASSERTMSG (pool_.slot_size () > 0, "OpenAL error: No sources can't be created.");
}


AudioChannelPool::~AudioChannelPool ()
{
	for (ChannelList::const_iterator i = pool_.slot_begin (); i != pool_.slot_end (); ++i)
	{
		AudioChannel* const channel = (*i);
		delete channel;
	}

	pool_.clear ();
}


AudioChannel* AudioChannelPool::SourceReserve (Sound* const sound_base)
{
	if (!pool_.full ())
	{
		AudioChannel* const channel = pool_.ReserveSlot ();
		channel->Reserve (sound_base);
		return channel;
	}


	// All channels are busy, so try to find lower priority slot.
	if (!pool_.empty ())
	{
		//AudioChannel* const channel = pool_.GetMinItem ();

		ChannelList::const_iterator iter = std::min_element (pool_.begin (), pool_.end (), AudioChannel::CompareByPriority);
		AudioChannel* const channel = (*iter);

		if (channel->GetPriority () < sound_base->GetPriority ())
		{
			DBG_MSG ("Channel Get : All channels used, interrupted priority %d for %d", channel->GetPriority (), sound_base->GetPriority ());
			channel->Cancel ();

			if (!pool_.full ())
			{
				AudioChannel* const channel = pool_.ReserveSlot ();
				channel->Reserve (sound_base);
				return channel;
			}
		}
	}


	DBG_MSG ("Channel : can't reserve.");
	//DBG_MSG ("Audio reserve:\ntotal: %d\nused: %d\nfree: %d", channel_arr_.size (), used_arr_.size (), free_arr_.size ());

	return NULL;
}


void AudioChannelPool::SourceRelease (AudioChannel* const channel)
{
	channel->Release ();
	pool_.ReleaseSlot (channel);

	//DBG_MSG ("Audio release:\ntotal: %d\nused: %d\nfree: %d", channel_arr_.size (), used_arr_.size (), free_arr_.size ());
}


#if (0)
// @TODO: I saw some system doing sleep like below, in our case we disable Mixer.
void AudioChannelPool::Sleep ()
{
	for (SoundChannelList::const_iterator i = used_arr_.begin (); i != used_arr_.end (); ++i)
	{
		AudioChannel* const channel = (*i);
		const ALuint source = channel->source_;
		ALint state;
		alGetSourcei (source, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
		{
			alSourcePause (source);
			channel->is_sleeping_ = true;
		}
	}
}


void AudioChannelPool::Awake ()
{
	for (SoundChannelList::const_iterator i = used_arr_.begin (); i != used_arr_.end (); ++i)
	{
		AudioChannel* const channel = (*i);
		if (channel->is_sleeping_)
		{
			alSourcePlay (channel->source_);
			channel->is_sleeping_ = false;
		}
	}
}
#endif

