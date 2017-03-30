#pragma once

#include "audio_api.h"
#include <stdio.h>
#include <list>
#include <algorithm>

#include "audio_channel.h"
#include "pool.h"


class Sound;
class AudioChannel;


class AudioChannelPool
{
	typedef std::list <AudioChannel*> 	ChannelList;


public:

	AudioChannelPool ();
	~AudioChannelPool ();
	AudioChannel*				SourceReserve (Sound* const sound_base);
	void						SourceRelease (AudioChannel* const src);


#if (0)
	void						Sleep ();
	void						Awake ();
#endif


private:

	Pool <AudioChannel>			pool_;
};

