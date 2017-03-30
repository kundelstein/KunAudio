#pragma once

#include "engine_config.h"
#include "mathlib.h"
#include <functional>


enum class SoundAction
{
	CANCEL = 0,
	START,
	FINISH,
	PAUSE,
	RESUME,
	NUM
};


class Sound;


class SoundState
{
public:

	SoundState (const SoundAction action)
		: action_ (action)
	{
	}


public:

	SoundAction					action_;
};


typedef std::function<void (const Sound* const sound, const SoundState& state)> SoundCallback;


