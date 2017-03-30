#include "scene_test_sound_effect.h"


namespace
{
	static int soundId = 0;

	enum
	{
		CHANNEL_MAX_PRIORITY = 255
	};
}


void SceneTestSoundEffect::OnEnter ()
{
	mixer_ = new Mixer ();


	sound_file_1_ = new AudioEffectFile ("snd/sound_short_1.ogg");
	sound_file_2_ = new AudioEffectFile ("snd/sound_short_2.ogg");
	sound_file_3_ = new AudioEffectFile ("snd/sound_short_3.ogg");
	sound_effect_1_ = new SoundEffect (sound_file_1_, 1, mixer_);
	sound_effect_2_ = new SoundEffect (sound_file_2_, 2, mixer_);
	sound_effect_3_ = new SoundEffect (sound_file_3_, 3, mixer_);

	sound_effect_1_->SetCallback (SoundAction::START, CALLBACK_2 (SceneTestSoundEffect::onFeedbackStart1, this));
	sound_effect_1_->SetCallback (SoundAction::CANCEL, CALLBACK_2 (SceneTestSoundEffect::onFeedbackCancel1, this));
	sound_effect_1_->SetCallback (SoundAction::FINISH, CALLBACK_2 (SceneTestSoundEffect::onFeedbackFinish1, this));

	sound_effect_2_->SetCallback (SoundAction::START, CALLBACK_2 (SceneTestSoundEffect::onFeedbackStart2, this));
	sound_effect_2_->SetCallback (SoundAction::CANCEL, CALLBACK_2 (SceneTestSoundEffect::onFeedbackCancel2, this));
	sound_effect_2_->SetCallback (SoundAction::FINISH, CALLBACK_2 (SceneTestSoundEffect::onFeedbackFinish2, this));

	sound_effect_3_->SetCallback (SoundAction::START, CALLBACK_2 (SceneTestSoundEffect::onFeedbackStart3, this));
	sound_effect_3_->SetCallback (SoundAction::CANCEL, CALLBACK_2 (SceneTestSoundEffect::onFeedbackCancel3, this));
	sound_effect_3_->SetCallback (SoundAction::FINISH, CALLBACK_2 (SceneTestSoundEffect::onFeedbackFinish3, this));

	sound_effect_1_->SetVolume (0.05f);
	sound_effect_2_->SetVolume (0.05f);
	sound_effect_3_->SetVolume (0.05f);


	sound_stream_ = new SoundStream ("snd/sound_loop.ogg", 0, mixer_);
	sound_stream_->SetLoop (true);

	sound_stream_->SetCallback (SoundAction::START, CALLBACK_2 (SceneTestSoundEffect::onStreamFeedbackStart, this));
	sound_stream_->SetCallback (SoundAction::CANCEL, CALLBACK_2 (SceneTestSoundEffect::onStreamFeedbackCancel, this));
	sound_stream_->SetCallback (SoundAction::FINISH, CALLBACK_2 (SceneTestSoundEffect::onStreamFeedbackFinish, this));


	sound_stream_1_ = new SoundStream ("snd/sound_long_1.ogg", CHANNEL_MAX_PRIORITY, mixer_);
	sound_stream_2_ = new SoundStream ("snd/sound_long_2.ogg", CHANNEL_MAX_PRIORITY, mixer_);
	sound_stream_3_ = new SoundStream ("snd/sound_long_3.ogg", CHANNEL_MAX_PRIORITY, mixer_);

	sound_stream_1_->SetVolume (0.05f);
	sound_stream_2_->SetVolume (0.05f);
	sound_stream_3_->SetVolume (0.05f);


	sound_stream_->Play ();
}


void SceneTestSoundEffect::OnLeave ()
{
	delete sound_stream_3_;
	delete sound_stream_2_;
	delete sound_stream_1_;
	delete sound_stream_;

	delete sound_effect_3_;
	delete sound_effect_2_;
	delete sound_effect_1_;

	delete sound_file_3_;
	delete sound_file_2_;
	delete sound_file_1_;

	delete mixer_;
}


void SceneTestSoundEffect::OnUpdate (const double time_dt)
{
}


void SceneTestSoundEffect::OnRender ()
{
}


void SceneTestSoundEffect::onFeedbackStart1 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: start1");
}


void SceneTestSoundEffect::onFeedbackCancel1 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: cancel1");
}


void SceneTestSoundEffect::onFeedbackFinish1 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: finish1");
}


void SceneTestSoundEffect::onFeedbackStart2 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: start2");
}


void SceneTestSoundEffect::onFeedbackCancel2 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: cancel2");
}


void SceneTestSoundEffect::onFeedbackFinish2 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: finish2");
}


void SceneTestSoundEffect::onFeedbackStart3 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: start3");
}


void SceneTestSoundEffect::onFeedbackCancel3 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: cancel3");
}


void SceneTestSoundEffect::onFeedbackFinish3 (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: finish3");
}


void SceneTestSoundEffect::onStreamFeedbackStart (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: start");
}


void SceneTestSoundEffect::onStreamFeedbackCancel (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: cancel");
}


void SceneTestSoundEffect::onStreamFeedbackFinish (const Sound* const sound, const SoundState state)
{
	DBG_MSG ("Feedback: finish");
}


