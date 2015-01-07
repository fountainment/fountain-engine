#ifndef _FT_AUDIO_H_
#define _FT_AUDIO_H_

namespace ftAudio {
	
bool init();

int loadSound(const char *filename);

//TODO: design a Channel class to make sound
// class Channel {
// private:
// public:
// };

};

#endif