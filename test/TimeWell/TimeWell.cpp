#include <fountain.h>
#include "scenes.h"

using namespace fountain;

void fountain::setBasicVarible()
{
	mainWin.title = "TimeWell";
	mainWin.setSize(800, 600);
	mainWin.icon = "timewell.ico";
	mainWin.hideCursor = true;
	mainWin.isFullScreen = true;
}

void fountain::gameInit()
{
	ftRender::setClearColor(FT_Black);
	ftPhysics::setRatio(100.0f);
	sceneSelector.gotoScene(new GameScene());
}

void fountain::singleFrame()
{
	mainClock.tick();
	sceneSelector.doAll();
}
