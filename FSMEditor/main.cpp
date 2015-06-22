#include <fountain.h>
#include "editor.h"

ftType::FontMan fontman;

void fountain::setBasicVarible()
{
	mainWin.setSize(1366, 768);
	mainWin.title = std::string("FSM Editor");
	mainWin.isFullScreen = true;
	mainWin.hideCursor = true;
}

void fountain::gameInit()
{
	ftRender::openLineSmooth();
	ftRender::setLineWidth(2);
	fontman.loadFont("resources/font/test.ttc");
	fontman.genAsciiTable(32);
	ftUI::setDefaultFont(&fontman);
	ftRender::setClearColor(ftColor("#333"));
	fountain::sceneSelector.gotoScene(new editorScene());
}

void fountain::singleFrame()
{
	fountain::mainClock.tick();
	fountain::sceneSelector.doAll();
}
