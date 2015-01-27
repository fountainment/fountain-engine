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
	ftPhysics::setRatio(64.0f);
	fm.loadFont("res/font/wqy.ttc");
	lf.loadFont("res/font/wqy.ttc");
	fm.genAsciiTable(128.0f);
	lf.genAsciiTable(32.0f);
	ftUI::setDefaultFont(&fm);
	sceneSelector.gotoScene(new OpenScene());
}

void fountain::singleFrame()
{
	mainClock.tick();
	sceneSelector.doAll();
}
