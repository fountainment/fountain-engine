#include <fountain.h>
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
	ftRender::openLineSmooth();
	glLineWidth(1.5f);
	ftRender::openPointSmooth();

	mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	mainClock.init();
//	Game::testPic = ftRender::getPicture("test.jpg");
//	glEnable(GL_DEPTH_TEST);

	std::vector<ftVec2> v;
	v.push_back(ftVec2(0,-2));
	v.push_back(ftVec2(1,0));
	v.push_back(ftVec2(0,2));
	v.push_back(ftVec2(-1,0));
	testShape0 = new Shape(v , 4, true);
	testShape1 = new Shape();
	testShape2 = new Shape(1);
	groundBox = new Shape(ftRect(0, 0, 100, 1));
	rect = new Shape(ftRect(0, 0, 1, 1));
	card = new Shape(ftRect(0, 0, 0.3, 3));
	addShape = rect;

	for (int i = 0; i < 10; i++) {
		for (int j = 0 - i; j <= i; j++) {
			bdPoint = new ftPhysics::Body(j, -i, false);
			bdPoint->shape = *addShape;
			if (!mainWorld.addBody(bdPoint)) {
				mainWorld.delHeadBody();
				mainWorld.addBody(bdPoint);
			}
		}
	}

	bdPoint = new ftPhysics::Body(0, -100, false);
	bdPoint->shape = *groundBox;
	if (!mainWorld.addBody(bdPoint)) {
		mainWorld.delHeadBody();
		mainWorld.addBody(bdPoint);
	}

	for (int i = -50; i <= 50; i+=2) {
		bdPoint = new ftPhysics::Body(i, -100 + 3.6);
		bdPoint->shape = *card;
		if (!mainWorld.addBody(bdPoint)) {
			mainWorld.delHeadBody();
			mainWorld.addBody(bdPoint);
		}
	}
	SS.registerScene(&startScene, FT_StartScene);
	SS.registerScene(&gameScene, FT_Scene2);
}

void fountain::singleFrame()
{
	SS.sceneSolve();
	SS.update();
	SS.draw();

	//TODO: move this internal(fountainMain)
	mainClock.tick();
}
