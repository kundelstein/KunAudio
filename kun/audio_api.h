#pragma once

#include "engine_config.h"
#include "debug.h"


#if (SYSTEM_WINDOWS)

#include <AL/al.h>
#include <AL/alc.h>

#else

#import <UIKit/UIKit.h>

#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <OpenAl/al.h>
#import <OpenAl/alc.h>
#include <AudioToolbox/AudioToolbox.h>

#endif


void AudioErrorCallback (const char* stmt, const char* fname, int line);


#if (DEBUG_AUDIO_API)

#define AL_CHECK() do \
{ \
	ALenum  error = alGetError (); \
	if (error != AL_NO_ERROR) { \
		DBG_MSG ( "** %s:%i ** ERROR:  %x", __FILE__, __LINE__, error); \
	} \
} while (0);

#else

#define AL_CHECK() do \
{ \
	alGetError (); \
} while (0);

#endif


class Engine;


class AudioApi
{
public:

	AudioApi (Engine* const engine);
	~AudioApi ();

	void						Api_Suspend ();
	void						Api_Advance ();


private:

	void						Api_Enable ();
	void						Api_Disable ();


private:

	Engine*						engine_ptr_;
	ALCdevice*					device_;
	ALCcontext*					context_;
};

