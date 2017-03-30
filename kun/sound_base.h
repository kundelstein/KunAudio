#pragma once

#include "engine_config.h"
#include "priority_element.h"
#include "sound_event.h"
#include "sound_callback.h"
#include "playing_state.h"
#include "mathlib.h"
#include "debug.h"
#include <string>


class Mixer;
class AudioChannel;
class AudioChannelPool;


// @NOTE: The sound model is a bit different than that of OpenAL. The Mixer decides on how the model will be working, not individual sounds.
// @NOTE: In future there might be a situation when we have to tweak some parameters per sound, not per mixer (like AL_REFERENCE_DISTANCE).


class Sound : public Priorited<Sound>
{
	friend Mixer;
	friend Priorited;


public:

	Sound ();


protected:

	~Sound ();


public:

	void							Play ();
	void							PlayFrom (const float start_time);
	void							Cancel ();
	void							Pause ();
	void							Resume ();

	void							SetLoop (const bool loop);
	bool							GetLoop () const;

	void							SetVolume (const float volume);
	float							GetVolume ();

	void							SetPitch (const float pitch);
	float							GetPitch ();

	void							SetPos (const glm::vec3& pos);
	glm::vec3						GetPos () const;
	PlayingState					GetState () const;

	float							GetLengthSecond () const;
	int								GetLengthSample () const;


	void							SoundAddToMixer (Mixer* const mixer);
	void							SoundRemoveFromMixer ();

	void							SetCallback (const SoundAction action, const SoundCallback& callback);
	void							ClearCallback (const SoundAction action);

	bool							Update (const float dt);


private:

	virtual bool					InternalUpdate (const float dt) = 0;
	virtual void					InternalPlay () = 0;
	virtual void					InternalPlayFromSeconds (const float seconds) = 0;				// @TODO: Rethink this one to unify the system more.
	//virtual void					InternalPlayFromSamples (const unsigned int samples) = 0;
	virtual void					InternalStop () = 0;
	virtual void					InternalRewind () = 0;
	virtual void					InternalRemember () = 0;
	virtual void					InternalResume () = 0;
	virtual void					InternalAttachSource () = 0;
	virtual void					InternalUpdateLoop () = 0;

	void							UpdateVolume ();

	void							AttachSource ();
	void							DetachSource ();
	void							FinishedFeedback ();

	void							RegisterEvent (const SoundAction state);
	void							ClearCallbacks ();

	void							ManagerAddToActive ();
	void							ManagerRemoveFromActive ();
	void							ManagerCleanup ();

	void							PriorityChanged ();		// public Priorited


private:

	SoundCallback					callback_arr_[static_cast<int>(SoundAction::NUM)];

	Mixer*							mixer_ptr_;
	glm::vec3						pos_;
	float							volume_;
	float							pitch_;


protected:

	AudioChannel*					channel_ptr_;
	float							length_seconds_;
	int								length_samples_;
	PlayingState					state_;
	bool							is_loop_;
	bool							is_loaded_;

public:

	static AudioChannelPool*		source_atlas_;
};

