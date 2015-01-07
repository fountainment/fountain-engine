#include <fountain.h>

ftTime::Clock mainClock(60.0);
ftScene::SceneSelector SS;
ftRender::ShaderProgram SP("resources/shader/vs.vert", "resources/shader/fs.frag");
ftType::FontMan fm;

#include "testscene.h"
#include <string>

void fountain::setBasicVarible()
{
	mainWin.setW(800);
	mainWin.setH(600);
	mainWin.title = std::string("fountain-prototype ") + std::string(FOUNTAIN_VERSION);
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = false;
//	mainWin.isFullScreen = true;
	mainWin.hideCursor = false;
}

void fountain::gameInit()
{
	mainClock.init();

	SS.registerScene(&startScene, FT_StartScene);
	SS.registerScene(&gameScene, FT_Scene2);
	SS.registerScene(&modelScene, FT_Scene3);

	ftFile lData("resources/data/letters.dat");
	fm.loadFont("resources/font/test.ttc");
	fm.genStringTable(lData.getStr(), 32);

	SP.init();
	SP.setUniform("time", 0.0f);
	SP.use();
}

void fountain::singleFrame()
{
	//TODO: move this internal(fountainMain)
	mainClock.tick();

	ftVec2 mp = fountain::sysMouse.getPos();
	SP.setUniform("time", mainClock.secondsFromInit());
	SP.setUniform("resolution", fountain::getWinSize());
	SP.setUniform("mouse", mp);

	if (fountain::sysKeyboard.getState(FT_S) == FT_KeyUp)
		SP.use();
	if (fountain::sysKeyboard.getState(FT_F) == FT_KeyUp)
		ftRender::useFFP();
	if (fountain::sysKeyboard.getState(FT_R) == FT_KeyUp)
		SP.reload();

	SS.sceneSolve();
	SS.update();
	SS.draw();

	//test
	ftRender::transformBegin();
	  ftRender::ftTranslate(-300, 0);
	  fm.drawString("Welcome to Fountain Game Engine!");
	ftRender::transformEnd();

	ftRender::transformBegin();
	  ftRender::ftTranslate(-300, -100);
	  fm.drawString("你好，这里是Fountain游戏引擎！");
	ftRender::transformEnd();
}
