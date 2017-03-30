#include "engine.h"
#include "audio_api.h"
#include "mixer.h"
#include "mixer_manager.h"
#include "audio_channel_atlas.h"
#include "sound_base.h"
#include "debug.h"
#include <chrono>

#include "scene_test_sound_effect.h"


Engine::Engine ()
{
	audio_api_ = new AudioApi (this);
	Mixer::manager_ = new MixerManager (this);
	Sound::source_atlas_ = new AudioChannelPool ();

	scene_ = new SceneTestSoundEffect ();
	scene_->OnEnter ();
}


Engine::~Engine ()
{
	scene_->OnLeave ();
	delete scene_;

	delete Sound::source_atlas_;
	delete Mixer::manager_;
	delete audio_api_;
}


void Engine::Update (const double time_dt)
{
	Mixer::manager_->ManagerUpdateMixer (time_dt);
	scene_->OnUpdate (time_dt);
}


void Engine::Render ()
{
}

