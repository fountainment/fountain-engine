#include <fountain.h>
#define abs(x) ((x)>0?(x):-(x))

container<ftVec3, 100000> con;

void drawPot(ftVec3 vec)
{
	glPointSize(2.0f);
	glColor3f(abs(vec.xyz[0] / 100.0f), abs(vec.xyz[1] / 100.0f), abs(vec.xyz[2] / 100.0f));
	glBegin(GL_POINTS);
	glVertex3fv(vec.xyz);
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
	mainWin.isFullScreen = false;
	mainWin.hideCursor = true;
}

void fountain::gameInit()
{
	//ftRender::openLineSmooth();
	//ftRender::openPointSmooth();
	Game::mainCamera.setProjectionType(FT_PERSPECTIVE);
	Game::mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	Game::testPic = ftRender::getPicture("test.jpg");
	Game::mainClock.init();
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < 100000; i++) {
		con.add(ftVec3(ftAlgorithm::randRangef(-100, 100), ftAlgorithm::randRangef(-100, 100), ftAlgorithm::randRangef(-100, 100)));
	}
}

void fountain::singleFrame()
{
	Game::mainCamera.update();
	Game::deltaV = fountain::sysMouse.getDeltaV();
	Game::xAngle -= Game::deltaV.y;
	Game::yAngle += Game::deltaV.x;
	if (fountain::sysMouse.getState(1))
		Game::scale *= 1.1f;
	if (fountain::sysKeyboard.getState(FT_W))
		Game::scale += 5.0f * Game::mainClock.getDeltaT();
	if (fountain::sysMouse.getState(3))
		Game::scale /= 1.1f;
	if (fountain::sysKeyboard.getState(FT_S))
		Game::scale -= 5.0f * Game::mainClock.getDeltaT();

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
