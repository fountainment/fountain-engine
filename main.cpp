#include <fountain.h>

#include "testscene.h"
#include <string>

void fountain::setBasicVarible()
{
	mainWin.setSize(800, 600);
	mainWin.title = std::string("fountain-prototype ") + std::string(FOUNTAIN_VERSION);
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = false;
	mainWin.hideCursor = false;
}

void fountain::gameInit()
{
}

void fountain::singleFrame()
{
	//fountain::mainClock.tick();
	fountain::sceneSelector.doAll();
}
