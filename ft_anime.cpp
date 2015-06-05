#include <fountain/fountaindef.h>

using ftAnime::Anime;
using ftAnime::FrameAnime;

bool ftAnime::init()
{
	return true;
}

void ftAnime::close()
{

}

//class ftAnime::Anime
ftTime::Clock* Anime::getMasterClock()
{
	return masterClock;
}

void Anime::setMasterClock(ftTime::Clock *clock)
{
	masterClock = clock;
}

void Anime::play()
{
	if (getMasterClock() != NULL) {
		animeClock = ftTime::Clock(getMasterClock());
	} else {
		animeClock = ftTime::Clock(&fountain::mainClock);
	}
	animeClock.init();
	setState(0.0);
}

void Anime::pause()
{
	animeClock.pause();
}

void Anime::resume()
{
	animeClock.resume();
}

void Anime::stop()
{
	state = -1;
	animeClock.init();
	animeClock.pause();
}

bool Anime::isPlay()
{
	return state >= 0;
}

void Anime::setState(double st)
{
	state = st;
}

double Anime::getState()
{
	return state;
}

double Anime::getDeltaT()
{
	return deltaT;
}

void Anime::updateTime()
{
	animeClock.tick();
	curTime = animeClock.getTotalT();
	deltaT = animeClock.getDeltaT();
}

void Anime::update() {}
void Anime::draw() {}

void Anime::setLoop(bool lp)
{
	loop = lp;
}

bool Anime::isLoop()
{
	return loop;
}

//class ftAnime::FrameAnime
FrameAnime::FrameAnime()
{
	totalTime = 0.0;
	setState(-1.0);
	totalTime = 0.0;
	totalFrame = 1.0;
	curFrame = 0;
	setLoop(false);
	setMasterClock(NULL);
}

FrameAnime::FrameAnime(ftRender::SubImagePool sip, double fps)
{
	setState(-1.0);
	curFrame = 0;
	animeImage = sip;
	totalFrame = animeImage.getImageNumber();
	totalTime = totalFrame / fps;
	setLoop(false);
	setMasterClock(NULL);
}

void FrameAnime::update()
{
	updateTime();
	if (isPlay()) {
		setState(getState() + getDeltaT() / totalTime);
		curFrame = (int)(totalFrame * getState());
		if (curFrame >= totalFrame) {
			if (isLoop()) {
				double tmp = getState();
				play();
				setState(tmp - 1.0);
				curFrame = (int)(totalFrame * getState());
			} else {
				stop();
			}
		}
	}
}

void FrameAnime::draw()
{
	if (isPlay()) {
		ftRender::SubImage curImage = animeImage.getImageFromIndex(curFrame);
		ftRender::drawImage(curImage);
	}
}
