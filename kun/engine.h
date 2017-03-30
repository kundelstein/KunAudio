#pragma once

#include "engine_config.h"
#include "mathlib.h"
#include <stdio.h>
#include <string>


class AudioApi;
class SceneTestSoundEffect;


class Engine
{
public:

	Engine ();
	~Engine ();
	
	void							Update (const double time_dt);
	void							Render ();

	
private:

	AudioApi*						audio_api_;
	SceneTestSoundEffect*			scene_;
};

