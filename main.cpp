#include "fountain.h"

namespace Game {
	ftTime::Clock mainClock(60.0);
	ft3DModel::ObjModel uglyFace("first.obj");
}

void fountain::gameInit()
{
	fountain::init();
	Game::mainClock.init();
}

void fountain::singleFrame()
{
	Game::uglyFace.render();
	Game::mainClock.tick();
}

