#include "audio_effect_file.h"
#include "audio_api.h"
#include "debug.h"
#include <stb/stb_vorbis.h>


AudioEffectFile::AudioEffectFile (const std::string& filename)
	: buffer_id_ (0)
{
	// Load the data:

	short* data = NULL;

	length_samples_ = stb_vorbis_decode_filename (filename.c_str (), &channel_num_, &sample_rate_, &data);
	DBG_ASSERTMSG (length_samples_ > 0, "Audio file '%s' couldn't be found.", filename.c_str ());

	length_seconds_ = static_cast<float> (length_samples_) / static_cast<float> (sample_rate_);


	// Attach to OpenAL:

	const ALenum format = (channel_num_ == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	DBG_WARNINGMSG (channel_num_ == 1, "OGG Warning: OGG File '%s' is not MONO.", filename.c_str ());


	alGetError ();
	alGenBuffers (1, &buffer_id_);
	if (alGetError () != AL_NO_ERROR)
	{
		DBG_ERRORMSG ("OpenAL Error: Buffer could not be generated for file'%s'.", filename.c_str ());
		free (data); //delete[] data;
		return;
	}

	alGetError ();
	alBufferData (buffer_id_, format, data, length_samples_ * sizeof (short), sample_rate_);

	free (data); //delete[] data;


	if (alGetError () != AL_NO_ERROR)
	{
		alDeleteBuffers (1, &buffer_id_);
		DBG_ERRORMSG ("OpenAL Error: Buffer could not be filled for file '%s'.", filename.c_str ());
		return;
	}
}


AudioEffectFile::~AudioEffectFile ()
{
	alDeleteBuffers (1, &buffer_id_);
}


float AudioEffectFile::GetLengthSeconds () const
{
	return length_seconds_;
}


ALuint AudioEffectFile::GetBuffer () const
{
	return buffer_id_;
}

