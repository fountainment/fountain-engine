#include <fountain/fountaindef.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <cstdio>
#include <iostream>
#include <string>

using ftAudio::Channel;

ALCdevice *dev;
ALCcontext *ctx;

ALfloat listenerPos[] = {0.0, 0.0, 4.0};
ALfloat listenerVel[] = {0.0, 0.0, 0.0};
ALfloat	listenerOri[] = {0.0, 0.0, 1.0, 0.0, 1.0, 0.0};

ALfloat sourcePos[] = {0.0, 0.0, 0.0};
ALfloat sourceVel[] = {0.0, 0.0, 0.0};

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		FT_LOG(_msg "\n");	\
		return -1;		\
	}

/*
* Special thanks to Oliver Plunkett
* http://www.dunsanyinteractive.com/blogs/oliver/?p=72
*/

/*
* Struct that holds the RIFF data of the Wave file.
* The RIFF data is the meta data information that holds,
* the ID, size and format of the wave file
*/
struct RIFF_Header {
	char chunkID[4];
	int chunkSize; //size not including chunkSize or chunkID
	char format[4];
};

/*
* Struct to hold fmt subchunk data for WAVE files.
*/
struct WAVE_Format {
	char subChunkID[4];
	int subChunkSize;
	short audioFormat;
	short numChannels;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSample;
};

/*
* Struct to hold the data of the wave file
*/
struct WAVE_Data {
	char subChunkID[4]; //should contain the word data
	int subChunk2Size; //Stores the size of the data block
};

/*
* Load wave file function. No need for ALUT with this
*/
bool loadWavFile(const std::string filename, ALuint* buffer,
                 ALsizei* size, ALsizei* frequency, ALenum* format) {
	//Local Declarations
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;
	size_t tmp;

	try {
		soundFile = fopen(filename.c_str(), "rb");
		if (!soundFile)
			throw ("File not found!");

		// Read in the first chunk into the struct
		tmp = fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		//check for RIFF and WAVE tag in memeory
		if ((riff_header.chunkID[0] != 'R' ||
		        riff_header.chunkID[1] != 'I' ||
		        riff_header.chunkID[2] != 'F' ||
		        riff_header.chunkID[3] != 'F') ||
		        (riff_header.format[0] != 'W' ||
		         riff_header.format[1] != 'A' ||
		         riff_header.format[2] != 'V' ||
		         riff_header.format[3] != 'E'))
			throw ("Invalid RIFF or WAVE Header");

		//Read in the 2nd chunk for the wave info
		tmp = fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
		//check for fmt tag in memory
		if (wave_format.subChunkID[0] != 'f' ||
		        wave_format.subChunkID[1] != 'm' ||
		        wave_format.subChunkID[2] != 't' ||
		        wave_format.subChunkID[3] != ' ')
			throw ("Invalid Wave Format!");

		//check for extra parameters;
		if (wave_format.subChunkSize > 16)
			fseek(soundFile, sizeof(short), SEEK_CUR);

		//Read in the the last byte of data before the sound file
		tmp = fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
		if (!tmp) {};
		//check for data tag in memory
		if (wave_data.subChunkID[0] != 'd' ||
		        wave_data.subChunkID[1] != 'a' ||
		        wave_data.subChunkID[2] != 't' ||
		        wave_data.subChunkID[3] != 'a')
			throw ("Invalid data header!");

		//Allocate memory for data
		data = new unsigned char[wave_data.subChunk2Size];

		// Read in the sound data into the soundData variable
		if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
			throw ("Error loading WAVE data into struct!");

		//Now we set the variables that we passed in with the
		//data from the structs
		*size = wave_data.subChunk2Size;
		*frequency = wave_format.sampleRate;
		//The format is worked out by looking at the number of
		//channels and the bits per sample.
		if (wave_format.numChannels == 1) {
			if (wave_format.bitsPerSample == 8 )
				*format = AL_FORMAT_MONO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_MONO16;
		} else if (wave_format.numChannels == 2) {
			if (wave_format.bitsPerSample == 8 )
				*format = AL_FORMAT_STEREO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_STEREO16;
		}
		//create our openAL buffer and check for success
		alGenBuffers(1, buffer);
		//errorCheck();
		//now we put our data into the openAL buffer and
		//check for success
		alBufferData(*buffer, *format, (void*)data,
		             *size, *frequency);
		//errorCheck();
		//clean up and return true if successful
		delete [] data;
		fclose(soundFile);
		return true;
	} catch(const char *error) {
		//our catch statement for if we throw a string
		FT_ERROR("ftAudio: Loading \"%s\": %s\n", filename.c_str(), error);
		//clean up memory if wave loading fails
		if (soundFile != NULL)
			fclose(soundFile);
		//return false to indicate the failure to load wave
		return false;
	}
}

bool ftAudio::init()
{
	dev = alcOpenDevice(NULL);
	if (!dev) {
		FT_OUT("Oops! Sound device not found!\n");
		return false;
	}
	ctx = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(ctx);
	if (!ctx) {
		FT_OUT("Oops! Context create failed!\n");
		return false;
	}
	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);
	return true;
}

void ftAudio::close()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);
	alcCloseDevice(dev);
}

//class ftAudio::Channel
Channel::Channel()
{
}

Channel::~Channel()
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}

void Channel::init()
{
	alGenSources((ALuint)1, &source);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcefv(source, AL_POSITION, sourcePos);
	alSourcefv(source, AL_VELOCITY, sourceVel);
	setLoop(false);
}

bool Channel::load(const char *filename)
{
	ALsizei size, frequency;
	ALenum format;
	bool state = loadWavFile(filename, &buffer, &size, &frequency, &format);
	if (!state) {
		return false;
	}
	alSourcei(source, AL_BUFFER, buffer);
	return true;
}

void Channel::play()
{
	alSourcePlay(source);
}

void Channel::pause()
{
	alSourcePause(source);
}

void Channel::stop()
{
	alSourceStop(source);
}

void Channel::setLoop(bool loop)
{
	if (loop) {
		alSourcei(source, AL_LOOPING, AL_TRUE);
	} else {
		alSourcei(source, AL_LOOPING, AL_FALSE);
	}
}
