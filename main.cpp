#include <fountain.h>

container<ftVec2, 10000> con;

void drawPot(ftVec2 vec)
{
	glPointSize(3.0f);
	glColor3f(ftAlgorithm::randRangef(0.0f, 1.0f), ftAlgorithm::randRangef(0.0f, 1.0f), ftAlgorithm::randRangef(0.0f, 1.0f));
	glBegin(GL_POINTS);
	glVertex3f(vec.x, vec.y, ftAlgorithm::randRangef(-100, 100));
	glEnd();
}

namespace Game {
	double xAngle = 0.0f;
	double yAngle = 0.0f;
	double scale = 1.0f;
	int testPic;
	ftVec2 deltaV;
	 ftTime::Clock mainClock(60.0);
	 ft3DModel::ObjModel simpleModel("first.obj");
	 ftRender::Camera mainCamera(0, 0, 500);
};

void fountain::setBasicVarible()
{
	mainWin.setW(800);
	mainWin.setH(600);
	mainWin.title = "fountain-prototype 0.03";
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = true;
	mainWin.hideCursor = true;
}

void fountain::gameInit()
{
	ftRender::openLineSmooth();
	Game::mainCamera.setProjectionType(FT_PERSPECTIVE);
	glLineWidth(1.5f);
	Game::mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	Game::testPic = ftRender::getPicture("test.jpg");
	Game::mainClock.init();
	for (int i = 0; i < 10000; i++) {
		con.add(ftVec2(ftAlgorithm::randRangef(-100, 100), ftAlgorithm::randRangef(-100, 100)));
	}
}

void fountain::singleFrame()
{
	Game::mainCamera.update();
	Game::deltaV = fountain::sysMouse.getDeltaV();
	Game::xAngle -= Game::deltaV.y;
	Game::yAngle += Game::deltaV.x;
	if (fountain::sysMouse.getState(1))
		Game::scale += 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysKeyboard.getState(FT_W))
		Game::scale += 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysMouse.getState(3))
		Game::scale -= 40.0f * Game::mainClock.getDeltaT();
	if (fountain::sysKeyboard.getState(FT_S))
		Game::scale -= 40.0f * Game::mainClock.getDeltaT();

	ftRender::transformBegin();	
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::ftScale(Game::scale);
	con.doWith(drawPot);
	ftRender::transformEnd();

/*
	ftRender::transformBegin();	
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::ftScale(Game::scale);
	Game::simpleModel.render();
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::drawPic(Game::testPic);
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::drawLine(ftAlgorithm::randRangef(-110, -90), -100, 100, 100);
	ftRender::transformEnd();
*/
	Game::mainClock.tick();
}
