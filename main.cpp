#include <fountain.h>
#include <string>
#define abs(x) ((x)>0?(x):-(x))

ftPhysics::Body* bdPoint;

namespace Game {

double xAngle = 0.0f;
double yAngle = 0.0f;
double scale = 1.0f;
//int testPic;
ftVec2 deltaV;

//TODO: move this internal(ftTime)
ftTime::Clock mainClock(60.0);

//ft3DModel::ObjModel simpleModel("first.obj");

//TODO: move this internal(ftRender)
ftRender::Camera mainCamera(0, 0, 500);
};

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
	Game::mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	//Game::testPic = ftRender::getPicture("test.jpg");
	Game::mainClock.init();
	//glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < 10; i++) {
		for (int j = 0 - i; j <= i; j++) {
			bdPoint = new ftPhysics::Body(j, -i, false);
			if (!fountain::mainWorld.addBody(bdPoint)) {
				fountain::mainWorld.delHeadBody();
				fountain::mainWorld.addBody(bdPoint);
			}
		}
	}

	bdPoint = new ftPhysics::Body(0, -100, false);
	bdPoint->setRectSize(ftVec2(100, 1));
	if (!fountain::mainWorld.addBody(bdPoint)) {
		fountain::mainWorld.delHeadBody();
		fountain::mainWorld.addBody(bdPoint);
	}

	for (int i = -50; i <= 50; i+=2) {
		bdPoint = new ftPhysics::Body(i, -100 + 3.6);
		bdPoint->setRectSize(ftVec2(0.5, 3));
		if (!fountain::mainWorld.addBody(bdPoint)) {
			fountain::mainWorld.delHeadBody();
			fountain::mainWorld.addBody(bdPoint);
		}
	}
}

void fountain::singleFrame()
{
	ftVec2 mPos = fountain::sysMouse.getPos();
	mPos = Game::mainCamera.mouseToWorld(mPos);

	//TODO: move this internal
	fountain::mainWorld.update(Game::mainClock.getDeltaT());

	//TODO: move these to update(ftScene hook)
	if (fountain::sysMouse.getState(1)) {
		bdPoint = new ftPhysics::Body(mPos.x, mPos.y);
		if (!fountain::mainWorld.addBody(bdPoint)) {
			fountain::mainWorld.delHeadBody();
			fountain::mainWorld.addBody(bdPoint);
		}
	}

	if (fountain::sysMouse.getState(3)) {
		bdPoint = new ftPhysics::Body(mPos.x, mPos.y, false);
		if (!fountain::mainWorld.addBody(bdPoint)) {
			fountain::mainWorld.delHeadBody();
			fountain::mainWorld.addBody(bdPoint);
		}
	}
	if (fountain::sysMouse.getState(4)) {
		Game::scale *= 1.15f;
	}
	if (fountain::sysMouse.getState(5)) {
		Game::scale /= 1.15f;
	}
	if (fountain::sysMouse.getState(2)) {
		Game::deltaV = fountain::sysMouse.getDeltaV();
		Game::mainCamera.move(-Game::deltaV.x / Game::scale, -Game::deltaV.y / Game::scale);
	}

	Game::mainCamera.setScale(Game::scale);
	Game::mainCamera.update();

	fountain::mainWorld.draw();

	//TODO: move this internal(fountainMain)
	Game::mainClock.tick();
}
