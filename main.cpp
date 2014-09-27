#include "fountain.h"

namespace Game {
	float xAngle = 0.0f;
	float yAngle = 0.0f;
	float zAngle = 0.0f;
	ftTime::Clock mainClock(60.0);
	ft3DModel::ObjModel simpleModel("first.obj");
	ftRender::Camera mainCamera(100, 100, 500);
}

void fountain::gameInit()
{
	fountain::init();
	Game::mainClock.init();
	Game::mainCamera.setWinSize(800, 600);
}

void fountain::singleFrame()
{
	Game::mainCamera.setAngle(Game::xAngle, Game::yAngle, Game::zAngle);
	Game::mainCamera.update();
	Game::simpleModel.render();
	Game::zAngle -= 1;
	//Game::yAngle += 1;
	Game::mainClock.tick();
}

