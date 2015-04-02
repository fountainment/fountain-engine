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
	totalTime = 0.0;
	state = -1.0;
	totalTime = 0.0;
	totalFrame = 1.0;
	curFrame = 0;
	curTime = 0.0;
	loop = false;
}

FrameAnime::FrameAnime(ftRender::SubImagePool sip, double fps)
{
	state = -1.0;
	curFrame = 0;
	curTime = 0.0;
	animeImage = sip;
	totalFrame = animeImage.getImageNumber();
	totalTime = totalFrame / fps;
	loop = false;
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
	state = 0.0;
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

void FrameAnime::setState(double st)
{
	state = st;
}

double FrameAnime::getState()
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
			if (loop) {
				double tmp = state;
				play();
				state = tmp - 1.0;
			} else {
				stop();
			}
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

void FrameAnime::setLoop(bool lp)
{
	loop = lp;
}
