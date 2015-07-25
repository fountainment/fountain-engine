#include <fountain/fountaindef.h>

using ftAnime::Anime;
using ftAnime::FrameAnime;
using ftAnime::AFSM;

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
	init(sip, fps);
}

void FrameAnime::init(ftRender::SubImagePool sip, double fps)
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

//class AFSM(Animation Finite State Machine)
AFSM::AFSM()
{
	std::memset(use, 0, sizeof use);
	std::memset(layer, 0, sizeof layer);
	std::memset(fsm, -1, sizeof fsm);
	std::memset(curAnime, -1, sizeof curAnime);
	setMasterClock(NULL);
}

void AFSM::regAnime(int index, Anime* ani, bool lp, int lay)
{
	FT_ASSERT(index >= 0 && index < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(lay >= 0 && lay < FT_MAXLAYER, "FT_MAXLAYER");
	anime[index] = ani;
	anime[index]->setLoop(lp);
	use[index] = 1;
	layer[index] = lay;
}

void AFSM::unregAnime(int index)
{
	use[index] = 0;
	for (int i = 0; i < FT_MAXANIME; i++) {
		if (use[i]) {
			for (int j = 0; j < FT_MAXSIGNAL; j++) {
				if (fsm[i][j] == index)
					fsm[i][j] = -1;
			}
		}
	}
}

void AFSM::addConnection(int a, int signal, int b)
{
	FT_ASSERT(use[a] && use[b], "Anime must be registered!");
	FT_ASSERT(a >= 0 && a < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(b >= 0 && b < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(signal >= 0 && signal < FT_MAXSIGNAL, "FT_MAXSIGNAL");
	fsm[a][signal] = b;
}

int AFSM::getConnection(int a, int signal)
{
	FT_ASSERT(a >= 0 && a < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(signal >= 0 && signal < FT_MAXSIGNAL, "FT_MAXSIGNAL");
	return fsm[a][signal];
}

void AFSM::delConnection(int a, int signal)
{
	FT_ASSERT(a >= 0 && a < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(use[a], "Anime must be registered!");
	FT_ASSERT(signal >= 0 && signal < FT_MAXSIGNAL, "FT_MAXSIGNAL");
	fsm[a][signal] = -1;
}

void AFSM::setLoop(int index, bool loop)
{
	FT_ASSERT(index >= 0 && index < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(use[index], "AFSM: must reg first!")
	anime[index]->setLoop(loop);
}

bool AFSM::isLoop(int index)
{
	FT_ASSERT(index >= 0 && index < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(use[index], "AFSM: must reg first!")
	return anime[index]->isLoop();
}

void AFSM::setLayer(int index, int lay)
{
	FT_ASSERT(index >= 0 && index < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(use[index], "AFSM: must reg first!")
	FT_ASSERT(lay >= 0 && lay < FT_MAXLAYER, "FT_MAXLAYER");
	layer[index] = lay;
}

int AFSM::getLayer(int index)
{
	FT_ASSERT(index >= 0 && index < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(use[index], "AFSM: must reg first!")
	return layer[index];
}

void AFSM::inputSignal(int signal, int lay)
{
	FT_ASSERT(signal >= 0 && signal < FT_MAXSIGNAL, "FT_MAXSIGNAL");
	FT_ASSERT(lay >= 0 && lay < FT_MAXLAYER, "FT_MAXLAYER");
	int goAnime = fsm[curAnime[lay]][signal];
	if (goAnime == -1) {
		if (signal == 0) {
			curAnime[lay] = -1;
		}
		return;
	}
	anime[goAnime]->setMasterClock(getMasterClock());
	anime[goAnime]->play();
	curAnime[layer[goAnime]] = goAnime;
}

void AFSM::update()
{
	for (int i = 0; i < FT_MAXLAYER; i++) {
		if (curAnime[i] >= 0 && use[curAnime[i]]) {
			anime[curAnime[i]]->update();
			if (!anime[curAnime[i]]->isPlay()) {
				inputSignal(0, i);
			}
		}
	}
}

void AFSM::draw()
{
	for (int i = 0; i < FT_MAXLAYER; i++) {
		if (curAnime[i] >= 0 && use[curAnime[i]]) {
			anime[curAnime[i]]->draw();
		}
	}
}

void AFSM::startWith(int index)
{
	FT_ASSERT(index >= 0 && index < FT_MAXANIME, "FT_MAXANIME");
	FT_ASSERT(use[index], "AFSM: must reg first!")
	anime[index]->setMasterClock(getMasterClock());
	anime[index]->play();
	curAnime[layer[index]] = index;
}

void AFSM::saveFSM(const char* filename)
{
	ftFile f;
	f.open(filename);
	f.write("%d %d\n", FT_MAXANIME, FT_MAXSIGNAL);
	int useN = 0;
	for (int i = 0; i < FT_MAXANIME; i++) {
		if (use[i]) useN++;
		for (int j = 0; j < FT_MAXSIGNAL; j++) {
			f.write("%d ", fsm[i][j]);
		}
		f.write("\n");
	}
	f.write("%d\n", useN);
	for (int i = 0; i < FT_MAXANIME; i++) {
		if (use[i]) {
			int t = 0;
			if (isLoop(i)) t = 1;
			f.write("%d %d %d\n", i, t, getLayer(i));
		}
	}
	f.close();
}

void AFSM::loadFSM(const char* filename)
{
	int ani, sig, useN;
	ftFile f;
	f.open(filename);
	f.read("%d%d", &ani, &sig);
	for (int i = 0; i < ani; i++) {
		for (int j = 0; j < sig; j++) {
			f.read("%d", &fsm[i][j]);
		}
	}
	int index, loop, lay;
	f.read("%d", &useN);
	for (int i = 0; i < useN; i++) {
		f.read("%d%d%d", &index, &loop, &lay);
		bool lp = true;
		if (loop == 0) lp = false;
		if (use[index]) {
			setLoop(index, lp);
			setLayer(index, lay);
		}
	}
	f.close();
}
