#include <fountain.h>
#include "scenes.h"

ftType::FontMan fm;

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
	ftFile lData("resources/data/letters.dat");
	fm.loadFont("resources/font/test.ttc");
	fm.genStringTable(lData.getStr(), 64);
	ftUI::setDefaultFont(&fm);
	ftRender::setClearColor(ftColor("#131313"));
	fountain::sceneSelector.gotoScene(new RakNetScene());
}

void fountain::singleFrame()
{
	fountain::mainClock.tick();
	fountain::sceneSelector.doAll();
}
