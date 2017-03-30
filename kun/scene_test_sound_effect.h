#pragma once

#include "kun.h"


class SceneTestSoundEffect
{
public:

	void			OnEnter ();
	void			OnLeave ();
	void			OnRender ();
	void			OnUpdate (const double time_dt);

private:

	void onFeedbackStart1 (const Sound* const sound, const SoundState state);
	void onFeedbackCancel1 (const Sound* const sound, const SoundState state);
	void onFeedbackFinish1 (const Sound* const sound, const SoundState state);

	void onFeedbackStart2 (const Sound* const sound, const SoundState state);
	void onFeedbackCancel2 (const Sound* const sound, const SoundState state);
	void onFeedbackFinish2 (const Sound* const sound, const SoundState state);

	void onFeedbackStart3 (const Sound* const sound, const SoundState state);
	void onFeedbackCancel3 (const Sound* const sound, const SoundState state);
	void onFeedbackFinish3 (const Sound* const sound, const SoundState state);

	void onStreamFeedbackStart (const Sound* const sound, const SoundState state);
	void onStreamFeedbackCancel (const Sound* const sound, const SoundState state);
	void onStreamFeedbackFinish (const Sound* const sound, const SoundState state);


private:

	Mixer* mixer_;

	AudioEffectFile* sound_file_1_;
	AudioEffectFile* sound_file_2_;
	AudioEffectFile* sound_file_3_;

	SoundEffect* sound_effect_1_;
	SoundEffect* sound_effect_2_;
	SoundEffect* sound_effect_3_;

	SoundStream* sound_stream_;
	SoundStream* sound_stream_1_;
	SoundStream* sound_stream_2_;
	SoundStream* sound_stream_3_;
};

