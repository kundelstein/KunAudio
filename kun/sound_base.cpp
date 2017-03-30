#include "sound_base.h"
#include "audio_channel_atlas.h"
#include "mixer.h"
#include "audio_channel.h"
#include "audio_api.h"


AudioChannelPool*	Sound::source_atlas_;


Sound::Sound ()
{
	ClearCallbacks ();

	channel_ptr_ = NULL;
	mixer_ptr_ = NULL;

	length_samples_ = 0;
	length_seconds_ = 0;

	volume_ = 1.0f;
	pitch_ = 1.0f;
	is_loop_ = false;
	is_loaded_ = false;

	state_ = PlayingState::STOPPED;
}


Sound::~Sound ()
{
}


void Sound::AttachSource ()
{
	DBG_ASSERT (!channel_ptr_);

	channel_ptr_ = source_atlas_->SourceReserve (this);
	if (!channel_ptr_)
	{
		DBG_MSG ("Can't allocate source player.");
		return;
	}

	const ALint source_ = channel_ptr_->source_;

	alSource3f (source_, AL_VELOCITY, 0, 0, 0);
	alSource3f (source_, AL_DIRECTION, 0, 0, 0);
	alSourcef (source_, AL_PITCH, 1.0f);

	alSource3f (source_, AL_POSITION, pos_.x, pos_.y, pos_.z);
	alSourcei (source_, AL_LOOPING, 0);
	alSourcef (source_, AL_GAIN, volume_ * mixer_ptr_->GetVolume ());
	alSourcef (source_, AL_PITCH, pitch_);

	alSourcei (source_, AL_SOURCE_RELATIVE, mixer_ptr_->GetRelative ());
	alSourcef (source_, AL_REFERENCE_DISTANCE, mixer_ptr_->GetReferenceDistance ());
	alSourcef (source_, AL_ROLLOFF_FACTOR, mixer_ptr_->GetRolloffFactor ());		// @TODO: Warning: the BUG in alDistanceModel (AL_NONE) so we need here 0, (status as for ios 3.1)!
	alSourcef (source_, AL_MAX_DISTANCE, mixer_ptr_->GetMaxDistance ());

	InternalAttachSource ();
}


void Sound::DetachSource ()
{
	DBG_ASSERT (channel_ptr_);

	source_atlas_->SourceRelease (channel_ptr_);
	channel_ptr_ = NULL;
}


bool Sound::Update (const float dt)
{
	const bool active = InternalUpdate (dt);

	if (!active)
	{
		FinishedFeedback ();	// @TODO: Teoretycznie sygnal zakonczenia moge puscic pietro wyzej (w mikserze).
	}

	return active;
}


void Sound::Play ()
{
	if (!mixer_ptr_)
		return;

	if (!mixer_ptr_->IsActive ())
		return;

	if (!is_loaded_)
		return;


	switch (state_)
	{
		case PlayingState::STOPPED:
		{
			AttachSource ();

			if (!channel_ptr_)
				return;

			state_ = PlayingState::PLAYING;
			InternalPlay ();
			RegisterEvent (SoundAction::START);

			ManagerAddToActive ();

			break;
		}
		break;

		case PlayingState::PAUSED:

			AttachSource ();

			if (!channel_ptr_)
				return;

			state_ = PlayingState::PLAYING;
			InternalPlay ();
			RegisterEvent (SoundAction::START);

			ManagerAddToActive ();

			break;

		case PlayingState::PLAYING:
		{
			DBG_ASSERT (channel_ptr_);

			InternalRewind ();
			RegisterEvent (SoundAction::START);

			break;
		}

		default:
			break;
	}
}


void Sound::PlayFrom (const float start_time)
{
	if (!mixer_ptr_)
		return;

	if (!mixer_ptr_->IsActive ())
		return;

	if (!is_loaded_)
		return;


	float time = start_time;
	if (is_loop_)
	{
		time = fmodf (time, length_seconds_);
	}


	switch (state_)
	{
		case PlayingState::STOPPED:
		{
			if (time >= length_seconds_)
			{
				RegisterEvent (SoundAction::START);
				RegisterEvent (SoundAction::CANCEL);
				break;
			}

			AttachSource ();

			if (!channel_ptr_)
				return;

			state_ = PlayingState::PLAYING;
			InternalPlayFromSeconds (time);
			RegisterEvent (SoundAction::START);

			ManagerAddToActive ();

			break;
		}
		case PlayingState::PAUSED:
		{
			if (time >= length_seconds_)
			{
				state_ = PlayingState::STOPPED;
				RegisterEvent (SoundAction::START);
				RegisterEvent (SoundAction::CANCEL);

				break;
			}

			AttachSource ();

			if (!channel_ptr_)
				return;

			state_ = PlayingState::PLAYING;
			InternalPlayFromSeconds (time);
			RegisterEvent (SoundAction::START);

			ManagerAddToActive ();

			break;
		}
		case PlayingState::PLAYING:
		{
			if (time >= length_seconds_)
			{
				state_ = PlayingState::STOPPED;
				InternalStop ();
				DetachSource ();
				RegisterEvent (SoundAction::START);
				RegisterEvent (SoundAction::CANCEL);

				ManagerRemoveFromActive ();

				break;
			}

			DBG_ASSERT (channel_ptr_);

			InternalPlayFromSeconds (time);		// @TODO: Zastanow sie, w starym systemie uzywalem InternalContinueFromSeconds (time) a to robilo troche inaczej.
			RegisterEvent (SoundAction::START);

			break;
		}
		default:
			break;
	}
}


void Sound::Cancel ()
{
	if (!mixer_ptr_)
		return;

	if (!mixer_ptr_->IsActive ())
		return;

	switch (state_)
	{
		case PlayingState::STOPPED:
			RegisterEvent (SoundAction::CANCEL);
			break;

		case PlayingState::PAUSED:

			state_ = PlayingState::STOPPED;
			RegisterEvent (SoundAction::CANCEL);

			break;

		case PlayingState::PLAYING:

			DBG_ASSERT (channel_ptr_);

			state_ = PlayingState::STOPPED;
			InternalStop ();
			DetachSource ();
			RegisterEvent (SoundAction::CANCEL);

			ManagerRemoveFromActive ();

			break;

		default:
			break;
	}
}


void Sound::Pause ()
{
	if (!mixer_ptr_)
		return;

	if (!mixer_ptr_->IsActive ())
		return;

	RegisterEvent (SoundAction::PAUSE);

	if (state_ != PlayingState::PLAYING)
		return;

	state_ = PlayingState::PAUSED;

	InternalRemember ();
	InternalStop ();
	DetachSource ();

	ManagerRemoveFromActive ();
}


void Sound::Resume ()
{
	if (!mixer_ptr_)
		return;

	if (!mixer_ptr_->IsActive ())
		return;

	if (!is_loaded_)
		return;

	RegisterEvent (SoundAction::RESUME);

	if (state_ != PlayingState::PAUSED)
		return;

	AttachSource ();

	if (!channel_ptr_)
		return;

	state_ = PlayingState::PLAYING;

	InternalResume ();

	ManagerAddToActive ();
}


void Sound::FinishedFeedback ()
{
	DetachSource ();

	state_ = PlayingState::STOPPED;
	RegisterEvent (SoundAction::FINISH);
}


void Sound::SetLoop (const bool loop)
{
	is_loop_ = loop;

	InternalUpdateLoop ();
}


bool Sound::GetLoop () const
{
	return is_loop_;
}


void Sound::SetVolume (const float volume)
{
	volume_ = glm::clamp (volume, 0.0f, 1.0f);

	if (!channel_ptr_)
		return;

	UpdateVolume ();
}


float Sound::GetVolume ()
{
	//alGetSourcef (channel_ptr_->source_, AL_GAIN, &volume_);			// @NOTE: Teoretycznie to nie jest potrzebne, jezeli zaden parametr przekazany do funkcji nie zostaje ograniczony przez limit. Ponadto mam mixer_volume.
	return volume_;
}


void Sound::UpdateVolume ()
{
	alSourcef (channel_ptr_->source_, AL_GAIN, volume_ * mixer_ptr_->GetVolume ());
}


void Sound::SetPitch (const float pitch)
{
	//volume_ = glm::clamp (volume, 0.0f, 1.0f);
	pitch_ = pitch;

	if (!channel_ptr_)
		return;

	alSourcef (channel_ptr_->source_, AL_PITCH, pitch_);
}


float Sound::GetPitch ()
{
	//alGetSourcef (channel_ptr_->source_, AL_PITCH, &pitch_);				// @NOTE: Teoretycznie to nie jest potrzebne, jezeli zaden parametr przekazany do funkcji nie zostaje ograniczony przez limit.
	return pitch_;
}


void Sound::SetPos (const glm::vec3& pos)
{
	pos_ = pos;

	if (!channel_ptr_)
		return;

	alSource3f (channel_ptr_->source_, AL_POSITION, pos_.x, pos_.y, pos_.z);
}


glm::vec3 Sound::GetPos () const
{
	return pos_;
}


PlayingState Sound::GetState () const
{
	return state_;
}


float Sound::GetLengthSecond () const
{
	return length_seconds_;
}


int Sound::GetLengthSample () const
{
	return length_samples_;
}


void Sound::ManagerCleanup ()
{
	mixer_ptr_->CleanEventBySound (this);
}


void Sound::SetCallback (const SoundAction action, const SoundCallback& callback)
{
	callback_arr_[static_cast<int>(action)] = callback;
}


void Sound::ClearCallback (const SoundAction action)
{
	callback_arr_[static_cast<int>(action)] = nullptr;
}


void Sound::ClearCallbacks ()
{
	for (int i = 0; i < static_cast<int>(SoundAction::NUM); ++i)
	{
		callback_arr_[i] = nullptr;
	}
}


void Sound::RegisterEvent (const SoundAction action)
{
	const SoundCallback& sound_callback = callback_arr_[static_cast<int>(action)];
	if (sound_callback)
	{
		mixer_ptr_->QueueSoundEvent (SoundEvent (this, SoundState (action), sound_callback));
	}
}


void Sound::SoundAddToMixer (Mixer* const mixer)
{
	if (!mixer)
		return;

	DBG_ASSERTMSG (!mixer_ptr_, "SoundStream already added to mixer.");
	DBG_ASSERTMSG (mixer, "Mixer can't be NULL.");

	mixer_ptr_ = mixer;
	mixer_ptr_->sound_cluster_.AddElement (this, state_ == PlayingState::PLAYING);					// @TODO: To jest bzdura, bo nie moge zrobic play a dopiero potem dodac.
}


void Sound::SoundRemoveFromMixer ()
{
	Cancel ();
	ManagerCleanup ();

	mixer_ptr_->sound_cluster_.RemoveElement (this, state_ == PlayingState::PLAYING);				// @TODO: To jest bzdura, bo nie moge zrobic play a dopiero potem dodac.
	mixer_ptr_ = NULL;
}


void Sound::PriorityChanged ()
{
	if (mixer_ptr_)
	{
		mixer_ptr_->sound_cluster_.SetElementPriorityDirtyFlag ();
	}
}


void Sound::ManagerAddToActive ()
{
	mixer_ptr_->sound_cluster_.AddElementToActiveArr (this);
}


void Sound::ManagerRemoveFromActive ()
{
	mixer_ptr_->sound_cluster_.RemoveElementFromActiveArr (this);
}

