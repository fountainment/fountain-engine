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

	ftVec2 mp = fountain::sysMouse.getPos();
	SP.setVarible("time", mainClock.secondsFromInit());
	SP.setVarible("resolution", fountain::getWinSize());
	SP.setVarible("mouse", mp);

	if (fountain::sysKeyboard.getState(FT_S) == FT_KeyUp)
		SP.use();
	if (fountain::sysKeyboard.getState(FT_F) == FT_KeyUp)
		ftRender::useFFP();
	if (fountain::sysKeyboard.getState(FT_R) == FT_KeyUp)
		SP.reload();

	SS.sceneSolve();
	SS.update();
	SS.draw();
}
