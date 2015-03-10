#include <fountain/fountaindef.h>

using ftAnime::FrameAnime;

bool ftAnime::init()
{
	return true;
}

void ftAnime::close()
{

}

//class ftAnime::FrameAnime
FrameAnime::FrameAnime()
{
	totalTime = 0.0f;
	state = -1.0f;
	totalTime = 0.0f;
	totalFrame = 1.0f;
	curFrame = 0;
	curTime = 0.0f;
}

FrameAnime::FrameAnime(ftRender::SubImagePool sip, float fps)
{
	state = -1.0f;
	curFrame = 0;
	curTime = 0.0f;
	animeImage = sip;
	totalFrame = animeImage.getImageNumber();
	totalTime = totalFrame / fps;
}

void FrameAnime::play(ftTime::Clock *playClock)
{
	if (playClock != NULL) {
		animeClock = ftTime::Clock(playClock);
	} else {
		ftScene::Scene *scene = fountain::sceneSelector.getCurScene();
		animeClock = ftTime::Clock(&(scene->mainClock));
	}
	animeClock.init();
	state = 0.0f;
	curFrame = 0;
}

void FrameAnime::pause()
{
	animeClock.pause();
}

void FrameAnime::resume()
{
	animeClock.resume();
}

void FrameAnime::stop()
{
	state = -1;
	animeClock.init();
	animeClock.pause();
}

bool FrameAnime::isPlay()
{
	return state >= 0;
}

void FrameAnime::setState(float st)
{
	state = st;
}

float FrameAnime::getState()
{
	return state;
}

void FrameAnime::update()
{
	animeClock.tick();
	if (state >= 0) {
		curTime = animeClock.getTotalT();
		state += animeClock.getDeltaT() / totalTime;
		curFrame = (int)(totalFrame * state);
		if (curFrame >= totalFrame) {
			float tmp = state;
			play();
			state = tmp - 1.0f;
		}
	}
}

void FrameAnime::draw()
{
	if (state >= 0) {
		ftRender::SubImage curImage = animeImage.getImageFromIndex(curFrame);
		ftRender::drawImage(curImage);
	}
}
