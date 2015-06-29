#include <fountain.h>
#include "editor.h"

void fountain::setBasicVarible()
{
	mainWin.setSize(800, 600);
	mainWin.title = std::string("Polygon Editor");
	mainWin.isFullScreen = true;
}

void fountain::gameInit()
{
	ftRender::openLineSmooth();
	ftRender::setLineWidth(2);
	ftRender::setClearColor(ftColor("#333"));
	fountain::sceneSelector.gotoScene(new editorScene());
}

void fountain::singleFrame()
{
	fountain::mainClock.tick();
	fountain::sceneSelector.doAll();
}
