#include "scenes.h"

#include <cstdio>

ftRender::ShaderProgram bg("res/shader/vs.vert", "res/shader/bg.frag");

void OpenScene::init()
{
	cursorID = ftRender::getPicture("res/image/cursor.png");
}

void OpenScene::update()
{
}

void OpenScene::draw()
{
	mainCamera.update();
	bg.use();
	bg.setUniform("time", mainClock.secondsFromInit());
	bg.setUniform("resolution", fountain::getWinSize());
	ftRender::drawQuad(fountain::mainWin.w, fountain::mainWin.h);
	ftRender::useFFP();
	ftRender::transformBegin();
	ftRender::ftTranslate(mainCamera.mouseToWorld(fountain::sysMouse.getPos()));
	ftRender::drawAlphaPic(cursorID);
	ftRender::transformEnd();
}
