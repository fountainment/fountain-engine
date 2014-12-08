#include <fountain.h>

ftTime::Clock mainClock(60.0);
ftScene::SceneSelector SS;

#include "testscene.h"
#include <string>

void fountain::setBasicVarible()
{
	mainWin.setW(800);
	mainWin.setH(600);
	mainWin.title = std::string("fountain-prototype ") + std::string(FOUNTAIN_VERSION);
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = false;
	mainWin.hideCursor = false;
}

void fountain::gameInit()
{
	//ftRender::openLineSmooth();
	//ftRender::openPointSmooth();
	//Game::testPic = ftRender::getPicture("test.jpg");
	//glEnable(GL_DEPTH_TEST);

	mainClock.init();

	SS.registerScene(&startScene, FT_StartScene);
	SS.registerScene(&gameScene, FT_Scene2);
	SS.registerScene(&modelScene, FT_Scene3);
}

void fountain::singleFrame()
{
	//TODO: move this internal(fountainMain)
	mainClock.tick();

	SS.sceneSolve();
	SS.update();
	SS.draw();
}
