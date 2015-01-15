#ifndef _FT_AUDIO_H_
#define _FT_AUDIO_H_

#include <AL/al.h>

namespace ftAudio {

bool init();
void close();

class Channel
{
private:
	ALuint source;
	ALuint buffer;
public:
	Channel();
	~Channel();
	void init();
	bool load(const char *filename);
	void play();
	void pause();
	void stop();
};

}

#endif
