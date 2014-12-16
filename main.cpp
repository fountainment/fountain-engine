#include <fountain.h>

ftTime::Clock mainClock(60.0);
ftScene::SceneSelector SS;
ftRender::ShaderProgram SP("resources/vs.vert", "resources/fs.frag");

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
	mainClock.init();

	SS.registerScene(&startScene, FT_StartScene);
	SS.registerScene(&gameScene, FT_Scene2);
	SS.registerScene(&modelScene, FT_Scene3);

	SP.init();
	SP.setVarible("time", 0.0f);
}

void fountain::singleFrame()
{
	//TODO: move this internal(fountainMain)
	mainClock.tick();

	SP.setVarible("time", mainClock.secondsFromInit());
	if (fountain::sysKeyboard.getState(FT_S) == FT_KeyUp)
		SP.use();
	if (fountain::sysKeyboard.getState(FT_F) == FT_KeyUp)
		ftRender::useFFP();

	SS.sceneSolve();
	SS.update();
	SS.draw();
}
