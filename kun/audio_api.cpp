#include "audio_api.h"
#include "engine.h"


const char* GetOpenALErrorString (const int errID)
{
	if (errID == AL_NO_ERROR) return "";
	if (errID == AL_INVALID_NAME) return " Invalid name ";
	if (errID == AL_INVALID_ENUM) return " Invalid enum ";
	if (errID == AL_INVALID_VALUE) return " Invalid value ";
	if (errID == AL_INVALID_OPERATION) return " Invalid operation ";
	if (errID == AL_OUT_OF_MEMORY) return " Out of memory like! ";

	return " Don't know ";
}


void AudioErrorCallback (const char* stmt, const char* fname, int line)
{
	ALenum err = alGetError ();
	if (err != AL_NO_ERROR)
	{
		DBG_MSG ("OpenAL error %08x, (%s) at %s:%i - for %s", err, GetOpenALErrorString (err), fname, line, stmt);
	}
};


AudioApi::AudioApi (Engine* const engine)
	: engine_ptr_ (engine)
{
	// OpenGL setup:
	device_ = alcOpenDevice (NULL);
	if (!device_)
	{
		DBG_ERRORMSG ("OpenAL error: Could not init OpenAL.");
		return;
	}


	context_ = alcCreateContext (device_, NULL);	// Context para alDev
	if (!context_)
	{
		DBG_ERRORMSG ("OpenAL error: Context can't be created.");
		return;
	}

	// Activate the system (AV and OpenGL).
	Api_Enable ();



	// @TODO: This should rather be moved to Mixer object, depending on game.
	{
		alDistanceModel (AL_LINEAR_DISTANCE_CLAMPED);
		//alDistanceModel (AL_NONE);

		const float volume = 1;
		alListenerf (AL_GAIN, volume);

		const glm::vec3 pos (0, 0, 40);
		alListener3f (AL_POSITION, pos.x, pos.y, pos.z);
	}


	// @TODO: Useful when we use more than a single context.
	//context_ = alcGetCurrentContext ();
	//device_ = alcGetContextsDevice (context_);
}


AudioApi::~AudioApi ()
{
	Api_Disable ();

	alcDestroyContext (context_);
	alcCloseDevice (device_);
}


void AudioApi::Api_Disable ()
{
	alcSuspendContext (context_);
	AL_CHECK ();
	
	alcMakeContextCurrent (NULL);
	AL_CHECK ();
}


void AudioApi::Api_Enable ()
{
	alcMakeContextCurrent (context_);
	AL_CHECK ();

	alcProcessContext (context_);
	AL_CHECK ();
}


void AudioApi::Api_Suspend ()
{
	Api_Disable ();
	
	alcSuspendContext (context_);
	AL_CHECK ();
	
	alcMakeContextCurrent (NULL);
	AL_CHECK ();
}


void AudioApi::Api_Advance ()
{
	alcMakeContextCurrent (context_);
	AL_CHECK ();

	alcProcessContext (context_);
	AL_CHECK ();

	Api_Enable ();
}

