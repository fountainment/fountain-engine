#include "fountain.h"

namespace Game {
	double xAngle = 0.0f;
	double yAngle = 0.0f;
	double scale = 1.0f;
	int testPic;
	ftVec2 deltaV;
	 ftTime::Clock mainClock(60.0);
	 ft3DModel::ObjModel simpleModel("first.obj");
	 ftRender::Camera mainCamera(0, 0, 500);
};

void fountain::setBasicVarible()
{
	mainWin.setW(800);
	mainWin.setH(600);
	mainWin.title = "fountain-prototype 0.03";
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = true;
	mainWin.hideCursor = true;
}

void fountain::gameInit()
{
	ftRender::openLineSmooth();
	Game::mainCamera.setProjectionType(FT_PERSPECTIVE);
	glLineWidth(1.5f);
	Game::mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	Game::testPic = ftRender::getPicture("test.jpg");
	Game::mainClock.init();
}

void fountain::singleFrame()
{
	Game::mainCamera.update();
	Game::deltaV = fountain::sysMouse.getDeltaV();
	Game::xAngle -= Game::deltaV.y;
	Game::yAngle += Game::deltaV.x;
	if (fountain::sysMouse.getState(1))
		Game::scale += 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysKeyboard.getState(FT_W))
		Game::scale += 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysMouse.getState(3))
		Game::scale -= 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysKeyboard.getState(FT_S))
		Game::scale -= 40.0f * Game::mainClock.getDeltaT();
/*
	ftRender::transformBegin();	
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::ftScale(Game::scale);
	Game::simpleModel.render();
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::drawPic(Game::testPic);
	ftRender::transformEnd();
*/
	ftRender::transformBegin();
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::drawLine(-100, -100, 100, 100);
	ftRender::transformEnd();

	Game::mainClock.tick();
}
