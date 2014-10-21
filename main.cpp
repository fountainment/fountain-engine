#include "fountain.h"

namespace Game {
	float xAngle = 0.0f;
	float yAngle = 0.0f;
	float scale = 1.0f;
	ftVec2 deltaV;
	ftTime::Clock mainClock(60.0);
	ft3DModel::ObjModel simpleModel("first.obj");
	ftRender::Camera mainCamera(0, 0, 500);
}

void fountain::setBasicVarible()
{
	mainWin.setW(800);
	mainWin.setH(600);
	mainWin.title = "fountain-prototype 0.03";
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = false;
	mainWin.hideCursor = false;
}

void fountain::gameInit()
{
	Game::mainClock.init();
}

void fountain::singleFrame()
{
	Game::mainCamera.update();
	Game::deltaV = fountain::sysMouse.getDeltaV();
	Game::xAngle -= Game::deltaV.y;
	Game::yAngle += Game::deltaV.x;
	if (fountain::sysMouse.getState(1)) Game::scale += 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysKeyboard.getState(FT_W)) Game::scale += 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysMouse.getState(3)) Game::scale -= 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysKeyboard.getState(FT_S)) Game::scale -= 40.0f * Game::mainClock.getDeltaT();
	glRotatef(Game::xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(Game::yAngle, 0.0f, 1.0f, 0.0f);
	glScalef(Game::scale, Game::scale, Game::scale);
	Game::simpleModel.render();
	Game::mainClock.tick();
}
