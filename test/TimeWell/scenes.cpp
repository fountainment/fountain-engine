#include "scenes.h"

#include <cstdio>

ftRender::ShaderProgram bg("res/shader/vs.vert", "res/shader/bg.frag");

void OpenScene::init()
{
}

void OpenScene::update()
{
}

void OpenScene::draw()
{
	bg.use();
	bg.setUniform("time", mainClock.secondsFromInit());
	bg.setUniform("resolution", fountain::getWinSize());
	ftRender::drawQuad(fountain::mainWin.w, fountain::mainWin.h);
	ftRender::useFFP();
}
