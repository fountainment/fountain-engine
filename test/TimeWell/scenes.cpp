#include "scenes.h"

ftRender::ShaderProgram bg("res/shader/vs.vert", "res/shader/bg.frag");

void BaseScene::init()
{
	screenC.setViewport(fountain::getWinRect());
	cursorID = ftRender::getPicture("res/image/cursor.png");
	otherInit();
}

void BaseScene::otherInit()
{
}

void BaseScene::update()
{
	otherUpdate();
}

void BaseScene::otherUpdate()
{
}

void BaseScene::draw()
{
	drawBG();
	otherDraw();
	drawCursor();
}

void BaseScene::drawBG()
{
	screenC.update();
	bg.use();
	bg.setUniform("time", mainClock.secondsFromInit());
	bg.setUniform("resolution", fountain::getWinSize());
	ftRender::drawQuad(fountain::mainWin.w, fountain::mainWin.h);
	ftRender::useFFP();
}

void BaseScene::otherDraw()
{
}

void BaseScene::drawCursor()
{
	screenC.update();
	ftRender::transformBegin();
	ftRender::ftTranslate(mainCamera.mouseToWorld(fountain::sysMouse.getPos()));
	ftRender::drawAlphaPic(cursorID);
	ftRender::transformEnd();
}

void OpenScene::otherInit()
{
}

void OpenScene::otherUpdate()
{
}

void OpenScene::otherDraw()
{
	mainCamera.update();
}
