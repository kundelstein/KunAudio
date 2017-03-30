#include "mixer.h"

#include "debug.h"
#include "sound_effect.h"
#include "sound_stream.h"
#include "sound_base.h"
#include "sound_event.h"
#include <algorithm> // For std::sort...


Mixer::Mixer (const int priority, const bool add)
	: Managed<MixerManager, Mixer> (priority)
{
	reference_distance_ = 1.0f;
	rolloff_factor_ = 1.0f;
	max_distance_ = std::numeric_limits<float>::max ();
	volume_ = 1.0f;
	is_relative_ = false;

	if (add)
	{
		AddToManaged ();
	}
}


Mixer::~Mixer ()
{
	DBG_ASSERTMSG (sound_cluster_.active_arr_.empty (), "Mixer element (Sound) not deleted, left: %d", sound_cluster_.active_arr_.size ());
	RemoveFromManaged ();
}


void Mixer::Update (const float dt)
{
	DBG_ASSERTMSG (paused_arr_.empty (), "Update while pause_arr is not empty! Something went wrong!");

	sound_cluster_.Update ();


	SoundList::const_iterator iter = sound_cluster_.active_arr_.begin ();
	while (iter != sound_cluster_.active_arr_.end ())
	{
		bool active = (*iter)->Update (dt);
		if (!active)
		{
			// @TODO: In theory I can call here finish signal.
			iter = sound_cluster_.active_arr_.erase (iter);				//active_arr_.erase(i++);  // alternatively, 
			continue;
		}

		++iter;
	}
}


void Mixer::QueueSoundEvent (const SoundEvent& event)
{
	DBG_ASSERT (manager_);
	manager_->ManagerQueueSoundEvent (event);
}


void Mixer::CleanEventBySound (Sound* const sound)
{
	DBG_ASSERT (manager_);
	manager_->ManagerCleanSoundEvent (sound);

	paused_arr_.remove (sound);	// @NOTE: I'm also removing it from the list of paused sounds.
	// @TODO: What if the Mixer is paused and I'm just adding sound, shouldn't I also add the sounds to paused_arr_ list?
}


void Mixer::EnableInternal ()
{
	manager_->EnableInternal (this);

	for (SoundList::const_iterator i = paused_arr_.begin (); i != paused_arr_.end (); ++i)
	{
		Sound* const iter = (*i);
		iter->Resume ();		// @TODO: Returns the signal, should I?
	}

	paused_arr_.clear ();
}


void Mixer::DisableInternal ()
{
	paused_arr_ = sound_cluster_.active_arr_;

	for (SoundList::const_iterator i = paused_arr_.begin (); i != paused_arr_.end (); ++i)
	{
		Sound* const iter = (*i);
		iter->Pause ();		// @TODO: Returns the signal, should I?
	}

	manager_->DisableInternal (this);
}


void Mixer::SetRelative (const bool relative)
{
	is_relative_ = relative;
}


bool Mixer::GetRelative () const
{
	return is_relative_;
}


void Mixer::SetReferenceDistance (const float val)
{
	reference_distance_ = val;
}


float Mixer::GetReferenceDistance () const
{
	return reference_distance_;
}


void Mixer::SetRolloffFactor (const float val)
{
	rolloff_factor_ = val;
}


float Mixer::GetRolloffFactor () const
{
	return rolloff_factor_;
}


void Mixer::SetMaxDistance (const float val)
{
	max_distance_ = val;
}


float Mixer::GetMaxDistance () const
{
	return max_distance_;
}


void Mixer::SetVolume (const float volume)
{
	volume_ = volume;

	//SoundList::const_iterator iter = sound_cluster_.active_arr_.begin ();
	//while (iter != sound_cluster_.active_arr_.end ())
	//{
	//	(*iter)->UpdateVolume ();
	//	++iter;
	//}

	for (SoundList::const_iterator i = sound_cluster_.active_arr_.begin (); i != sound_cluster_.active_arr_.end (); ++i)
	{
		Sound* const iter = (*i);
		iter->UpdateVolume ();
	}
}


float Mixer::GetVolume () const
{
	return volume_;
}

